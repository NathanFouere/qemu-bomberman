/*
 * Pagination.cpp
 *
 *  Created on: 27 oct. 2008
 *      Author: menaud
 */

/* Copyright (C) 2004  David Decotigny

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.
 */
#include "Pagination.h"
#include "MemoirePhysique.h"
#include "../MemoireTab.h"

extern MemoireTab maMemoirePhysique;

/**
 * Helper function to map a page in the pd.\ Suppose that the RAM
 * is identity mapped to resolve PT actual (CPU) address from the PD
 * entry
 */
 sextant_ret_t Pagination::paging_setup_map_helper(struct x86_pde * pd,
		paddr_t ppage,
		vaddr_t vaddr) {
	/* Get the page directory entry and table entry index for this
     address */
	unsigned index_in_pd = virt_to_pd_index(vaddr);
	unsigned index_in_pt = virt_to_pt_index(vaddr);

	/* Make sure the page table was mapped */
	struct x86_pte * pt;
	if (pd[index_in_pd].present)
	{
		pt = (struct x86_pte*) (pd[index_in_pd].pt_paddr << 12);

		/* If we allocate a new entry in the PT, increase its reference
	 count. This test will always be true here, since the setup
	 routine scans the kernel pages in a strictly increasing
	 order: at each step, the map will result in the allocation of
	 a new PT entry. For the sake of clarity, we keep the test
	 here. */
		if (! pt[index_in_pt].present) ;
			maMemoirePhysique.ref_physpage_at((paddr_t)pt);
	}
	else
	{
		/* No : allocate a new one */
		pt = (struct x86_pte*) maMemoirePhysique.malloc(false);
		if (! pt)
			return -SEXTANT_ERROR;

		memset((void*)pt, 0x0, PAGE_SIZE);

		pd[index_in_pd].present  = true;
		pd[index_in_pd].write    = 1; /* It would be too complicated to
				       determine whether it
				       corresponds to a real R/W area
				       of the kernel code/data or
				       read-only */
		pd[index_in_pd].pt_paddr = ((paddr_t)pt) >> 12;
	}


	/* Map the page in the page table */
	pt[index_in_pt].present = 1;
	pt[index_in_pt].write   = 1;  /* It would be too complicated to
				   determine whether it corresponds to
				   a real R/W area of the kernel
				   code/data or R/O only */
	pt[index_in_pt].user    = 0;
	pt[index_in_pt].paddr   = ppage >> 12;

	return SEXTANT_OK;
}


sextant_ret_t Pagination::paging_setup(paddr_t identity_mapping_base, paddr_t identity_mapping_top) {
	/* The PDBR we will setup below */
	struct x86_pdbr cr3;
	/* Get the PD for the kernel */
//	maMemoirePhysique=memoire;

	struct x86_pde * pd = (struct x86_pde*) maMemoirePhysique.malloc(false);


	/* The iterator for scanning the kernel area */
	paddr_t paddr;
	/* Reset the PD. For the moment, there is still an IM for the whole RAM, so that the paddr are also vaddr */
	memset((void*)pd,0x0,PAGE_SIZE);
	/* Identity-map the identity_mapping_* area */
	for(paddr = identity_mapping_base ;paddr < identity_mapping_top ;paddr += PAGE_SIZE) {
		if(paging_setup_map_helper(pd, paddr, paddr))
			return -SEXTANT_ERROR;
	}
	/* Identity-map the PC-specific BIOS/Video area */
	for(paddr = BIOS_N_VIDEO_START ; paddr < BIOS_N_VIDEO_END ; paddr += PAGE_SIZE) {
		if(paging_setup_map_helper(pd,paddr,paddr))
			return -SEXTANT_ERROR;
	}
	pd[virt_to_pd_index(PAGING_MIRROR_VADDR)].present = true;
	pd[virt_to_pd_index(PAGING_MIRROR_VADDR)].write = 1;
	pd[virt_to_pd_index(PAGING_MIRROR_VADDR)].user  = 0;
	pd[virt_to_pd_index(PAGING_MIRROR_VADDR)].pt_paddr = ((paddr_t)pd)>>12;

	/* We now just have to configure the MMU to use our PD. See Intel x86 doc vol 3, section 3.6.3 */
	memset(& cr3, 0x0, sizeof(struct x86_pdbr)); /* Reset the PDBR */
	cr3.pd_paddr = ((paddr_t)pd) >> 12;

	/* Actual loading of the PDBR in the MMU: setup cr3 + bits 31[Paging Enabled] and 16[Write Protect] of cr0, see Intel x86 doc vol 3,
    sections 2.5, 3.6.1 and 4.11.3 + note table 4-2 */
	asm volatile ("movl %0,%%cr3\n\t"
			"movl %%cr0,%%eax\n\t"
			"orl $0x80010000, %%eax\n\t" /* bit 31 | bit 16 */
			"movl %%eax,%%cr0\n\t"
			"jmp 1f\n\t"
			"1:\n\t"
			"movl $2f, %%eax\n\t"
			"jmp *%%eax\n\t"
			"2:\n\t" ::"r"(cr3):"memory","eax");
	/*
	 * Here, the only memory available is:
	 * - The BIOS+video area
	 * - the identity_mapping_base .. identity_mapping_top area
	 * - the PD mirroring area (4M)
	 * All accesses to other virtual addresses will generate a #PF
	 */
	return SEXTANT_OK;
}


/* Suppose that the current address is configured with the mirroring
 * enabled to access the PD and PT. */
sextant_ret_t Pagination::paging_map(paddr_t ppage_paddr,vaddr_t vpage_vaddr,bool is_user_page,ui32_t flags){
	/* Get the page directory entry and table entry index for this address */
	unsigned index_in_pd = virt_to_pd_index(vpage_vaddr);
	unsigned index_in_pt = virt_to_pt_index(vpage_vaddr);

	/* Get the PD of the current context */
	struct x86_pde* pd = (struct x86_pde*)(PAGING_MIRROR_VADDR + PAGE_SIZE*virt_to_pd_index(PAGING_MIRROR_VADDR));
	/* Address of the PT in the mirroring */
	struct x86_pte* pt = (struct x86_pte*)(PAGING_MIRROR_VADDR + PAGE_SIZE*index_in_pd);

	/* The mapping of anywhere in the PD mirroring is FORBIDDEN ;) */
	if((vpage_vaddr >= PAGING_MIRROR_VADDR) && (vpage_vaddr < PAGING_MIRROR_VADDR + PAGING_MIRROR_SIZE))
		return -SEXTANT_ERROR;

	/* Map a page for the PT if necessary */
	if(! pd[index_in_pd].present) {
		/* No : allocate a new one */
		paddr_t pt_ppage = maMemoirePhysique.malloc(!(flags & VM_MAP_ATOMIC));
		if(!pt_ppage)
			return -SEXTANT_ERROR;
		pd[index_in_pd].present  = true;
		pd[index_in_pd].write    = 1; /* Ignored in supervisor mode, see Intel vol 3 section 4.12 */
		pd[index_in_pd].user     |= (is_user_page)?1:0;
		pd[index_in_pd].pt_paddr = ((paddr_t)pt_ppage) >> 12;
		/* The PT is now mapped in the PD mirroring */
		/* Invalidate TLB for the page we just added */
		invlpg(pt);
		/* Reset this new PT */
		memset((void*)pt, 0x0, PAGE_SIZE);
	}

	/* If we allocate a new entry in the PT, increase its reference count. */
	else if (! pt[index_in_pt].present)
		maMemoirePhysique.ref_physpage_at(pd[index_in_pd].pt_paddr << 12);
	/* Otherwise, that means that a physical page is implicitely
     unmapped */
	else
		maMemoirePhysique.free(pt[index_in_pt].paddr << 12);
	/* Map the page in the page table */
	pt[index_in_pt].present = true;
	pt[index_in_pt].write   = (flags & VM_MAP_PROT_WRITE)?1:0;
	pt[index_in_pt].user    = (is_user_page)?1:0;
	pt[index_in_pt].paddr   = ppage_paddr >> 12;
	maMemoirePhysique.ref_physpage_at(ppage_paddr);
	/*
	 * The page is now mapped in the current address space
	 */
	/* Invalidate TLB for the page we just added */
	invlpg(vpage_vaddr);
	return SEXTANT_OK;
}

sextant_ret_t Pagination::paging_unmap(vaddr_t vpage_vaddr) {
	sextant_ret_t pt_unref_retval;
	/* Get the page directory entry and table entry index for this address */
	unsigned index_in_pd = virt_to_pd_index(vpage_vaddr);
	unsigned index_in_pt = virt_to_pt_index(vpage_vaddr);
	/* Get the PD of the current context */
	struct x86_pde *pd = (struct x86_pde*) (PAGING_MIRROR_VADDR + PAGE_SIZE*virt_to_pd_index(PAGING_MIRROR_VADDR));
	/* Address of the PT in the mirroring */
	struct x86_pte * pt = (struct x86_pte*) (PAGING_MIRROR_VADDR + PAGE_SIZE*index_in_pd);
	/* No page mapped at this address ? */
	if (! pd[index_in_pd].present)
		return -SEXTANT_ERROR;
	if (! pt[index_in_pt].present)
		return -SEXTANT_ERROR;
	/* The unmapping of anywhere in the PD mirroring is FORBIDDEN ;) */
	if ((vpage_vaddr >= PAGING_MIRROR_VADDR) && (vpage_vaddr < PAGING_MIRROR_VADDR + PAGING_MIRROR_SIZE))
		return -SEXTANT_ERROR;
	/* Reclaim the physical page */
	if (maMemoirePhysique.free(pt[index_in_pt].paddr << 12)==SEXTANT_ERROR) return -SEXTANT_ERROR;
	/* Unmap the page in the page table */
	memset(pt + index_in_pt, 0x0, sizeof(struct x86_pte));
	/* Invalidate TLB for the page we just unmapped */
	invlpg(vpage_vaddr);
	pt_unref_retval=1;
	/* Reclaim this entry in the PT, which may free the PT */
	pt_unref_retval = maMemoirePhysique.free(pd[index_in_pd].pt_paddr << 12);
	if(pt_unref_retval > 0) {/* If the PT is now completely unused... */
		/* Release the PDE */
		memset(pd + index_in_pd, 0x0, sizeof(struct x86_pde));
		/* Update the TLB */
		invlpg(pt);
	}
	return SEXTANT_OK;
}


int Pagination::paging_get_prot(vaddr_t vaddr) {
	int retval;
	/* Get the page directory entry and table entry index for this address */
	unsigned index_in_pd = virt_to_pd_index(vaddr);
	unsigned index_in_pt = virt_to_pt_index(vaddr);
	/* Get the PD of the current context */
	struct x86_pde *pd = (struct x86_pde*) (PAGING_MIRROR_VADDR + PAGE_SIZE*virt_to_pd_index(PAGING_MIRROR_VADDR));
	/* Address of the PT in the mirroring */
	struct x86_pte * pt = (struct x86_pte*) (PAGING_MIRROR_VADDR + PAGE_SIZE*index_in_pd);
	/* No page mapped at this address ? */
	if(!pd[index_in_pd].present)
		return VM_MAP_PROT_NONE;
	if(!pt[index_in_pt].present)
		return VM_MAP_PROT_NONE;
	/* Default access right of an available page is "read" on x86 */
	retval = VM_MAP_PROT_READ;
	if(pd[index_in_pd].write && pt[index_in_pt].write)
		retval |= VM_MAP_PROT_WRITE;
	return retval;
}

paddr_t Pagination::paging_get_paddr(vaddr_t vaddr) {
	/* Get the page directory entry and table entry index for this address */
	unsigned index_in_pd = virt_to_pd_index(vaddr);
	unsigned index_in_pt = virt_to_pt_index(vaddr);
	unsigned offset_in_page = virt_to_page_offset(vaddr);
	/* Get the PD of the current context */
	struct x86_pde *pd = (struct x86_pde*)(PAGING_MIRROR_VADDR + PAGE_SIZE*virt_to_pd_index(PAGING_MIRROR_VADDR));
	/* Address of the PT in the mirroring */
	struct x86_pte * pt = (struct x86_pte*)(PAGING_MIRROR_VADDR + PAGE_SIZE*index_in_pd);
	/* No page mapped at this address ? */
	if(!pd[index_in_pd].present)
		return (paddr_t)NULL;
	if(!pt[index_in_pt].present)
		return (paddr_t)NULL;
	return (pt[index_in_pt].paddr << 12) + offset_in_page;
}

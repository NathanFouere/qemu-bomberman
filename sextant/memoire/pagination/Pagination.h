/*
 * Pagination.h
 *
 *  Created on: 27 oct. 2008
 *      Author: menaud
 */

#ifndef PAGINATION_H_
#define PAGINATION_H_

#include <sextant/types.h>
//#include <sextant/memoire/pagination/MemoirePhysique.h>
#include <sextant/memoire/Memoire.h>
//#include <sextant/memoire/pagination/MemoirePhysique.h>
#include <sextant/memoire/op_memoire.h>

/** The structure of a page directory entry. See Intel vol 3 section 3.6.4 */
struct x86_pde {
	ui32_t present        :1; /* 1=PT mapped */
	ui32_t write          :1; /* 0=read-only, 1=read/write */
	ui32_t user           :1; /* 0=supervisor, 1=user */
	ui32_t write_through  :1; /* 0=write-back, 1=write-through */
	ui32_t cache_disabled :1; /* 1=cache disabled */
	ui32_t accessed       :1; /* 1=read/write access since last clear */
	ui32_t zero           :1; /* Intel reserved */
	ui32_t page_size      :1; /* 0=4kB, 1=4MB or 2MB (depending on PAE) */
	ui32_t global_page    :1; /* Ignored (Intel reserved) */
	ui32_t custom         :3; /* Do what you want with them */
	ui32_t pt_paddr       :20;
} __attribute__ ((packed));


/** The structure of a page table entry. See Intel vol 3 section 3.6.4 */
struct x86_pte {
	ui32_t present        :1; /* 1=PT mapped */
	ui32_t write          :1; /* 0=read-only, 1=read/write */
	ui32_t user           :1; /* 0=supervisor, 1=user */
	ui32_t write_through  :1; /* 0=write-back, 1=write-through */
	ui32_t cache_disabled :1; /* 1=cache disabled */
	ui32_t accessed       :1; /* 1=read/write access since last clear */
	ui32_t dirty          :1; /* 1=write access since last clear */
	ui32_t zero           :1; /* Intel reserved */
	ui32_t global_page    :1; /* 1=No TLB invalidation upon cr3 switch
				   (when PG set in cr4) */
	ui32_t custom         :3; /* Do what you want with them */
	ui32_t paddr          :20;
} __attribute__ ((packed));


/** Structure of the x86 CR3 register: the Page Directory Base
    Register. See Intel x86 doc Vol 3 section 2.5 */
struct x86_pdbr {
	ui32_t zero1          :3; /* Intel reserved */
	ui32_t write_through  :1; /* 0=write-back, 1=write-through */
	ui32_t cache_disabled :1; /* 1=cache disabled */
	ui32_t zero2          :7; /* Intel reserved */
	ui32_t pd_paddr       :20;
} __attribute__ ((packed));

/**
 * Helper macro to control the MMU: invalidate the whole TLB. See Intel x86 vol 3 section 3.7.
 */
#define flush_tlb() do { \
		unsigned long tmpreg; \
		asm volatile("movl %%cr3,%0\n\tmovl %0,%%cr3" :"=r" \
				(tmpreg) : :"memory"); \
	} while(0)

/**
 * Helper macro to compute the index in the PD for the given virtual address
 */
#define virt_to_pd_index(vaddr) (((unsigned)(vaddr)) >> 22)

/**
 * Helper macro to compute the index in the PT for the given virtual address
 */
#define virt_to_pt_index(vaddr) ((((unsigned)(vaddr)) >> 12) & 0x3ff )

/**
 * Helper macro to compute the offset in the page for the given virtual address
 */
#define virt_to_page_offset(vaddr) (((unsigned)(vaddr)) & PAGE_MASK)

/**
 * Helper macro to control the MMU: invalidate the TLB entry for the
 * page located at the given virtual address. See Intel x86 vol 3 section 3.7.
 */
#define invlpg(vaddr) \
	do { \
		__asm__ __volatile__("invlpg %0"::"m"(*((unsigned *)(vaddr)))); \
	} while(0)

/**
 * paging_map flags
 */
/** Usual virtual memory access rights */
#define VM_MAP_PROT_NONE  0
#define VM_MAP_PROT_READ  (1<<0)
#define VM_MAP_PROT_WRITE (1<<1)
/* EXEC not supported */
/** Mapping a page may involve an physical page allocation (for a new
    PT), hence may potentially block */
#define VM_MAP_ATOMIC     (1<<31)

/** Virtual address where the mirroring takes place */
#define PAGING_MIRROR_VADDR 0x3fc00000 /* 1GB - 4MB */
/** Length of the space reserved for the mirroring in the kernel
    virtual space */
#define PAGING_MIRROR_SIZE  (1 << 22)  /* 1 PD = 1024 Page Tables = 4MB */

/**
 * Setup initial page directory structure where the kernel is
 * identically-mapped, and the mirroring. This routine also
 * identity-maps the BIOS and video areas, to allow some debugging
 * text to be printed to the console. Finally, this routine installs
 * the whole configuration into the MMU.
 */

class Pagination {
private :

//	MemoirePhysique maMemoirePhysique;

	 sextant_ret_t paging_setup_map_helper(struct x86_pde * pd,
			paddr_t ppage,
			vaddr_t vaddr);
public:

sextant_ret_t paging_setup(paddr_t identity_mapping_base,paddr_t identity_mapping_top);

/**
 * Map the given physical page at the given virtual address in the
 * current address space.
 *
 * @note *IMPORTANT*: The physical page ppage_paddr *MUST* have been
 * referenced by the caller through either a call to
 * physmem_ref_physpage_new() or physmem_ref_physpage_at(). It
 * would work if this were untrue, but this would be INCORRECT (it is
 * expected that one is owning the page before mapping it, or
 * otherwise the page could have been stolen by an interrupt or
 * another thread).
 *
 * @param ppage_paddr  The address of a physical page (page-aligned)
 * @param vpage_vaddr  The address of the virtual page (page-aligned)
 * @param is_user_page TRUE when the page is available from user space
 * @param flags        A mask made of VM_* bits
 *
 * @note Unless the VM_MAP_ATOMIC bit is set in the flags, the
 * function may potentially block, because a physical page may be
 * allocated for a new PT.
 */
sextant_ret_t paging_map(paddr_t ppage_paddr,
			 vaddr_t vpage_vaddr,
			 bool is_user_page,
			 ui32_t flags);

/**
 * Undo the mapping from vaddr to the underlying physical page (if any)
 * @param vpage_vaddr  The address of the virtual page (page-aligned)
 */
sextant_ret_t paging_unmap(vaddr_t vpage_vaddr);

/**
 * Return the page protection flags (VM_MAP_PROT_*) associated
 * with the address, or VM_MAP_PROT_NONE when page is not mapped
 */
int paging_get_prot(vaddr_t vaddr);

/**
 * Return the physical address of the given virtual address. Since page
 * at physical addr 0 is not mapped, the NULL result means "page not
 * mapped".
 */
paddr_t paging_get_paddr(vaddr_t vaddr);

/**
 * Tell whether the address is physically mapped
 */
};

#define paging_check_present(vaddr) \
  (paging_get_paddr(vaddr) != NULL)


#endif /* PAGINATION_H_ */

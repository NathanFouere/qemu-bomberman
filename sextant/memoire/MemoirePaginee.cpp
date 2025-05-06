/*
 * MemoirePaginee.cpp
 *
 *  Created on: 24 oct. 2008
 *      Author: menaud
 */
//#include "memoire.h"
#include "MemoirePaginee.h"
#include "MemoireTab.h"
//#include "pagination/memoireliste4k.h"
//#include "pagination/paging.h"
#include <sextant/types.h>

extern paddr_t kernel_core_base,kernel_core_top;

vaddr_t free_adress;

//MemoirePhysique maMemoirePhysique;
MemoireTab maMemoirePhysique;

Pagination maPagination;

 void MemoirePaginee::testP()
{
  /* The (linear) address of the page holding the code we are
     currently executing */
  vaddr_t vpage_code = PAGE_ALIGN_INF(MemoirePaginee::testP);

  /* The new physical page that will hold the code */
  paddr_t ppage_new;

  /* Where this page will be mapped temporarily in order to copy the
     code into it: right after the kernel code/data */
  vaddr_t vpage_tmp = free_adress;

	free_adress = free_adress+PAGE_SIZE;

  /*
   * Test : move the page where we execute the code elsewhere in
   * physical memory
   */

  /* Allocate a new physical page */
  ppage_new = ( maMemoirePhysique).malloc(0);


  /* Map the page somewhere (right after the kernel mapping) in order
     to copy the code we are currently executing */
  maPagination.paging_map(ppage_new, vpage_tmp,
		 0,
		 VM_MAP_ATOMIC
		 | VM_MAP_PROT_READ
		 | VM_MAP_PROT_WRITE);

  /* Ok, the new page is referenced by the mapping, we can release our
     reference to it */
   maMemoirePhysique.free(ppage_new);

  /* Copy the contents of the current page of code to this new page
     mapping */
  memcpy((void*)vpage_tmp,
	 (void*)vpage_code,
	 PAGE_SIZE);

  /* Transfer the mapping of the current page of code to this new page */
  maPagination.paging_map(ppage_new, vpage_code,
		 0,
		 VM_MAP_ATOMIC
		 | VM_MAP_PROT_READ
		 | VM_MAP_PROT_WRITE);

  /* Ok, here we are: we have changed the physcal page that holds the
     code we are executing ;). However, this new page is mapped at 2
     virtual addresses:
     - vpage_tmp
     - vpage_code
     We can safely unmap it from sos_kernel_core_top_vaddr, while
     still keeping the vpage_code mapping */
  maPagination.paging_unmap(vpage_tmp);
}

void MemoirePaginee::test() {MemoirePaginee::testP();monEcran->afficherMot(2,10,"ca marche ",BLANC);}


void  MemoirePaginee::mem_setup(void *begin, int size,Ecran *ec){
		monEcran = ec;
		ec->afficherMot("MEMSETUP");
		maMemoirePhysique.mem_setup((void*)PAGE_ALIGN_SUP(begin),size,ec);
		maPagination.paging_setup(kernel_core_base,kernel_core_top);
		free_adress = PAGE_ALIGN_SUP(kernel_core_top);

}

vaddr_t MemoireTab::malloc(size_t nbytes) {
    if (nbytes > PAGE_SIZE) return SEXTANT_ERROR;

    for (int i = 0; i < MAXMEM; i++) {
        if (TabPages[i].isFree == true) {
            TabPages[i].isFree = false;
            vaddr_t vaddr = debut + i * PAGE_SIZE;
            return vaddr;
        }
    }
    return SEXTANT_ERROR;
}

sextant_ret_t MemoireTab::free(vaddr_t addr) {
    vaddr_t offset = addr - debut;
    int pageIndex = offset / PAGE_SIZE;

    if (pageIndex >= 0 && pageIndex < MAXMEM && TabPages[pageIndex].isFree == false) {
        TabPages[pageIndex].isFree = true; 
        return SEXTANT_OK;
    }
    return SEXTANT_ERROR; 
}

void  MemoirePaginee::memoireaffiche(Ecran *ec){


	monEcran->effacerEcran(NOIR);
	maMemoirePhysique.memoireaffiche(ec);


}


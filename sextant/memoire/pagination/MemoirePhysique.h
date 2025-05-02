/*
 * MemoirePhysique.h
 *
 *  Created on: 4 nov. 2008
 *      Author: menaud
 */

#ifndef MEMOIREPHYSIQUE_H_
#define MEMOIREPHYSIQUE_H_

/** The array of ppage descriptors will be located at this address */
#define PAGE_DESCR_ARRAY_ADDR PAGE_ALIGN_SUP((paddr_t) (& __e_kernel))

#define ALIGN_INF(val,boundary) (((unsigned)(val)) & (~((boundary)-1)))
/** Align on a boundary (MUST be a power of 2), so that return value >= val */
#define ALIGN_SUP(val,boundary) ({ unsigned int __bnd=(boundary); (((((unsigned)(val))-1) & (~(__bnd - 1))) + __bnd); })
/** The size of a physical page (arch-dependent) */
#define PAGE_SIZE  (4*1024)
/** The corresponding shift */
#define PAGE_SHIFT 12 /* 4 kB = 2^12 B */
/** The corresponding mask */
#define PAGE_MASK  ((1<<12) - 1)
#define PAGE_ALIGN_INF(val) ALIGN_INF((val), PAGE_SIZE)
#define PAGE_ALIGN_SUP(val) ALIGN_SUP((val), PAGE_SIZE)
#define IS_PAGE_ALIGNED(val) IS_ALIGNED((val), PAGE_SIZE)



/**
 * This is the reserved physical interval for the x86 video memory and
 * BIOS area. In physmem.c, we have to mark this area as "used" in
 * order to prevent from allocating it. And in paging.c, we'd better
 * map it in virtual space if we really want to be able to print to
 * the screen (for debugging purpose, at least): for this, the
 * simplest is to identity-map this area in virtual space (note
 * however that this mapping could also be non-identical).
 */
#define BIOS_N_VIDEO_START 0xa0000
#define BIOS_N_VIDEO_END   0x100000


#endif /* MEMOIREPHYSIQUE_H_ */

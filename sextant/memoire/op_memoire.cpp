#include "op_memoire.h"

void* memcpy(void *dst0, const void *src0, register unsigned int size) {
  char *dst;
  const char *src;
  for (dst = (char*)dst0, src = (const char*)src0 ;
       size > 0 ;
       dst++, src++, size--)
    *dst = *src;
  return dst0;
}

void* memset(void *dst0, register int c, register unsigned int length) {
  char *dst;
  for(dst = (char*) dst0 ;length > 0 ;dst++, length --)
    *dst = (char)c;
  return dst0;
}



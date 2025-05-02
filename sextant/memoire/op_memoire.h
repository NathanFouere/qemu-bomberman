#ifndef MEMORY
#define MEMORY

/***
 * Copie d'une zone memoire.
 ***/
void* memcpy(void *dst, const void *src, register unsigned int size );
/***
 * Remplissage de la memoire avec l'entier c.
 ***/
void* memset(void *dst, register int c, register unsigned int length ) ;

#endif


#include "Memoire.h"
#include "MemoireListe.h"

Ecran *ec;

#define DEBUG_MEMOIRELISTE 0

struct	mblock	{
	struct mblock* mnext;
	unsigned int mlen;
};

struct mblock memlist;

void MemoireListe::mem_setup(void* begin, int size,Ecran* ecran) {
	memlist.mnext = (mblock*) begin;
	memlist.mlen = size - (unsigned)begin;
	(memlist.mnext)->mlen= size - (unsigned)begin;
	ec=ecran;
}

void MemoireListe::memoireaffiche(Ecran *ec){

	mblock* bloc = &memlist;
	bloc = memlist.mnext;

	while (bloc!=NULL) {
	ec->afficherMot("\nBloc addr: ");
	ec->afficherBase((unsigned int) bloc,10,BLANC);
	ec->afficherMot(" | taille: ");
	ec->afficherBase((unsigned int) bloc->mlen,10,BLANC);
	ec->afficherMot("; bloc suiv: ");
	ec->afficherBase((unsigned int) bloc->mnext,10,BLANC);
	bloc=bloc->mnext;
	}

}

vaddr_t MemoireListe::malloc(size_t nbytes) {
	struct mblock *p, *q, *leftover;

	int *taille;

	nbytes=nbytes+4; // pour sauvegarder la taille


	if(nbytes==0 || memlist.mnext== (struct mblock *) NULL) {
		return(NULL);
	}
	//q pointe sur le bloc courant et p sur le bloc suivant
	for (q= &memlist,p=memlist.mnext ;p != (struct mblock *) NULL ;q=p,p=p->mnext)
		if(p->mlen == nbytes) {//On demande toute la mémoire
			q->mnext = p->mnext;

			taille=(int*)p;
			*taille=(nbytes-4);
			taille=taille+1;
			return( (vaddr_t)taille );

		} else if ( p->mlen > nbytes ) {
			leftover = (struct mblock *)( (unsigned)p + nbytes );//Recucupere le bloc de taille voulue
			q->mnext = leftover;//insertion du nouveau bloc dans la liste
			int temp1,temp2;
			temp1 = (int) p->mnext;
			temp2 = p->mlen - nbytes;
			leftover->mnext = (struct mblock*) temp1;
			leftover->mlen = temp2;

			taille=(int*)p;
			*taille=(nbytes-4);
			taille=taille+1;
			return((vaddr_t)taille);
		}
	return NULL;
}

sextant_ret_t MemoireListe::free(unsigned int addr) {
	struct mblock *p,*q;
	unsigned top;

	int *add = ((int*)addr)-1;
	struct mblock* block = (struct mblock*) add;

	int size = *add+4;


	if(size==0)
			return SEXTANT_ERROR;

	for( p=memlist.mnext,q= &memlist;
	     p != (struct mblock *) NULL && p < block ;
	     q=p,p=p->mnext )
		;
	if ((top=q->mlen+(unsigned)q)>(unsigned)block && q!= &memlist ||
	    p!=NULL && (size+(unsigned)block) > (unsigned)p ) {
		return(SEXTANT_ERROR);
	}
	if ( q!= &memlist && top == (unsigned)block )
			q->mlen += size;
	else {
		block->mlen = size;
		block->mnext = p;
		q->mnext = block;
		q = block;
	}
	if ( (unsigned)( q->mlen + (unsigned)q ) == (unsigned)p) {
		q->mlen += p->mlen;
		q->mnext = p->mnext;
	}
	return(SEXTANT_OK);

}

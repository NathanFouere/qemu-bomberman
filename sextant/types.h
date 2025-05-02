/* Copyright (C) 2004  The SOS Team
   Copyright (C) 1999  Free Software Foundation, Inc.

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
#ifndef TYPES_H
#define TYPES_H
/* Positive values of the error codes */
#define SEXTANT_OK            				0   /* No error */
#define SEXTANT_ERROR      					255 /* Internal fatal error */
#define PAGINATION_USE 						1	/*1 : activation de la pagination, 0 : gestion basique de la mémoire */
/** Not a proper type, but highly useful with basic type
    manipulations */
#define NULL 0
#define NULL1 ((void*)0)
#define NULL2 ((void(*)(int))0)
#define NULL_THREAD (thread *)0
/* For /dev/zero & /dev/null devices */
#define SEXTANT_CHARDEV_ZERO_MAJOR			1
#define SEXTANT_CHARDEV_ZERO_MINOR          0
#define SEXTANT_CHARDEV_NULL_MINOR          1

/* For memory char devices (/dev/mem and /dev/kmem) */
#define SEXTANT_CHARDEV_MEM_MAJOR           2
#define SEXTANT_CHARDEV_KMEM_MINOR          0 /* /dev/kmem */
#define SEXTANT_CHARDEV_PHYSMEM_MINOR       1 /* /dev/mem */

/* TTY major and minor*/
#define SEXTANT_CHARDEV_TTY_MAJOR           3
#define SEXTANT_CHARDEV_CONSOLE_MINOR       0
#define SEXTANT_CHARDEV_SERIAL_MINOR        1

/* TTY IOCTL commands */
#define SEXTANT_IOCTL_TTY_SETPARAM          1
#define SEXTANT_IOCTL_TTY_RESETPARAM        2

/* TTY IOCTL command parameters for SEXTANT_IOCTL_TTY_SETPARAM and
   SEXTANT_IOCTL_TTY_RESETPARAM */
#define SEXTANT_IOCTLPARAM_TTY_CANON        1
#define SEXTANT_IOCTLPARAM_TTY_ECHO         2

/** IDE major */
#define SEXTANT_BLOCKDEV_IDE_MAJOR          1

/** Generic hardrive IOCTL: flush caches to disk */
#define SEXTANT_IOCTL_BLOCKDEV_SYNC           1

#define SEXTANT_MYSELF 10

/**
 * @file types.h
 *
 * SOS basic types definition
 */
/* A negative value means that an error occured.  For
 *  example -EINVAL means that the error was "invalid
 * argument" */
typedef int sextant_ret_t;

/** Physical address */
typedef unsigned int       paddr_t;

/** Generic virtual address (kernel or user) */
typedef unsigned int       vaddr_t;

/** Memory size of an object (positive) */
typedef unsigned int size_t;

/** Low-level sizes */
typedef unsigned long long int ui64_t; /**< 32b unsigned */
typedef unsigned long int  ui32_t; /* 32b unsigned */
typedef unsigned short int ui16_t; /* 16b unsigned */
typedef unsigned char      ui8_t;  /* 8b unsigned */
typedef signed long int        si32_t; /**< 32b signed */
typedef signed long long int   si64_t; /**< 64b signed */
/** Generic count of objects */
typedef unsigned int           count_t;
/** Generic count of objects (LARGE) */
typedef unsigned long long int lcount_t;
typedef signed long long int   lsoffset_t;
/** User virtual address */
typedef unsigned int           uaddr_t;

/** Generic positive LONG (64 bits) offset into objects */
typedef unsigned long long int luoffset_t;
typedef unsigned long long int lcount_t;
/** Generic (Kernel or User) virtual address. We define a structure to
    force the compiler to signal when we call this special function
    with a usual address... */
typedef struct genaddr{
	bool is_user;
	ui32_t addr;
}genaddr_t;
/**
 * Couleurs pour l'écran.
 */
enum Couleur {NOIR,GRIS_SOMBRE,VERT,VERT_LEGER,
	ROUGE,ROUGE_LEGER,CYAN,CYAN_LEGER,BLEU,
	BLEU_LEGER,MAGENTA,MAGENTA_LEGER,MARRON,
	JAUNE,GRIS_LEGER,BLANC};

#endif

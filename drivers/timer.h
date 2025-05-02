/* Copyright (C) 2004  David Decotigny
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
#ifndef _i8259_H_
#define _i8259_H_

#include <sextant/types.h>
#include <hal/fonctionsES.h>

/**
 * @file i8254.h PC programmable timer
 *
 * Programmable timer routines. See the Intel 82C54 datasheet (on kos
 * website).
 *
 * @see i82C54 datasheet on Kos website.
 */
/** 82c54 clock frequency */
#define I8254_MAX_FREQ 1193180

/* Ports to communicate with the 82c54 */
#define I8254_TIMER0  0x40
#define I8254_TIMER1  0x41
#define I8254_TIMER2  0x42
#define I8254_CONTROL 0x43

/** Change timer interrupt (IRQ 0) frequency */

class Timer {

public :
	sextant_ret_t i8254_set_frequency(unsigned int freq);
	int getSecondes();
};



#endif /* _i8259_H_ */

/* Copyright (C) 2004  The KOS Team
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

#include "timer.h"

extern int secondes;


/**
 * Configure the first timer of the 82c54 chip as a rate generator,
 * which will raise an IRQ0 on a regular periodic basis, as given by
 * the freq parameter. Second (RAM refresh) and third (speaker) timers
 * are left unchanged. Maximum frequency is that of the 8254 clock, ie
 * 1193180 Hz.
 *
 * Ahhh PC systems are nice toys: this maximum "strange" frequency
 * equals that of the NTSC clock (14.31818 MHz) divided by 12. In
 * turn, the famous 4.77 MHz cpu clock frequency of the first IBM PC
 * is this same NTSC frequency divided by 3. Why the NTSC frequency as
 * a base "standard" ? Because the 14.31818 MHz quartz were cheap at
 * that time, and because it allows to simply drive altogether the
 * cpu, the "time of day" timer, and the video signal generators.
 */
sextant_ret_t Timer::i8254_set_frequency(unsigned int freq) {
  unsigned int nb_tick;

  if (freq <= 0)
    return -SEXTANT_ERROR;

  /* Compute counter value */
  nb_tick = I8254_MAX_FREQ / freq;

  /* Counter must be between 1 and 65536 */
  if (nb_tick > 65536)
    return -SEXTANT_ERROR;
  if (nb_tick <= 0)
    return -SEXTANT_ERROR;

  /* The i8254 interprets 0 to mean counter == 65536, because 65536
     cannot be coded on 16bits */
  if (nb_tick == 65536)
    nb_tick = 0;

  /* We want to configure timer0, we want to send both LSB+MSB to set
     timer0 freq (-> 0x30), and we configure timer0 in mode 2, ie as a
     rate generator (-> 0x4) ==> 0x34 */
  ecrireOctet(0x34, I8254_CONTROL);

  /* Send LSB of counter first */
  ecrireOctet((nb_tick & 0xFF), I8254_TIMER0);

  /* Send MSB of counter */
  ecrireOctet((nb_tick >> 8) & 0xFF, I8254_TIMER0);

  return SEXTANT_OK;
}

int Timer::getSecondes(){
	return secondes;
}


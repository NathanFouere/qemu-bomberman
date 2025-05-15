/*
 * handler_clavier.cpp
 *
 *  Created on: 8 ao�t 2008
 *      Author: jmenaud
 */
#include "handler_clavier.h"
#include <hal/fonctionsES.h>
#include <sextant/Synchronisation/Spinlock/Spinlock.h>
#include <drivers/vga.h>  // For debug visualization

int posBuf = 0;
char buf[256];
bool modifBuf = false;

static int ControlKeys = 0;
static char leds = 0;

static Spinlock keyboardLock;
static int spinlockVar = 1;  // Initialize the lock variable


int writechar(int sc) {
    int ch = 0;
    if ((ControlKeys & KBD_SHIFT) == 0)
        switch (sc) { // No SHIFT
        case 16: ch='a'; break;
        case 17: ch='z'; break;
        case 18: ch='e'; break;
        case 19: ch='r'; break;
        case 20: ch='t'; break;
        case 21: ch='y'; break;
        case 22: ch='u'; break;
        case 23: ch='i'; break;
        case 24: ch='o'; break;
        case 25: ch='p'; break;
        case 30: ch='q'; break;
        case 31: ch='s'; break;
        case 32: ch='d'; break;
        case 33: ch='f'; break;
        case 34: ch='g'; break;
        case 35: ch='h'; break;
        case 36: ch='j'; break;
        case 37: ch='k'; break;
        case 38: ch='l'; break;
        case 39: ch='m'; break;
        case 44: ch='w'; break;
        case 45: ch='x'; break;
        case 46: ch='c'; break;
        case 47: ch='v'; break;
        case 48: ch='b'; break;
        case 49: ch='n'; break;
        case 50: ch=','; break;

        case 2: ch='1'; break;
        case 3: ch='2'; break;
        case 4: ch='3'; break;
        case 5: ch='4'; break;
        case 6: ch='5'; break;
        case 7: ch='6'; break;
        case 8: ch='7'; break;
        case 9: ch='8'; break;
        case 10: ch='9'; break;
        case 11: ch='0'; break;
        case 57: ch=' '; break;
        case 14: ch=BS; break;
        case 28: ch=EN; break;

        case 185: ch=' '; break;
        case 142: ch=BS; break;
        case 156: ch=EN; break;

        case 59: ch=F1; break;
        case 60: ch=F2; break;
        case 61: ch=F3; break;
        case 62: ch=F4; break;
        case 63: ch=F5; break;
        case 64: ch=F6; break;
        case 65: ch=F7; break;
        case 66: ch=F8; break;
        case 67: ch=F9; break;
        case 68: ch=F10; break;

        default: ch=0; break;
        }
    else
        switch (sc) { // With SHIFT
        case 16: ch='A'; break;
        case 17: ch='Z'; break;
        case 18: ch='E'; break;
        case 19: ch='R'; break;
        case 20: ch='T'; break;
        case 21: ch='Y'; break;
        case 22: ch='U'; break;
        case 23: ch='I'; break;
        case 24: ch='O'; break;
        case 25: ch='P'; break;
        case 30: ch='Q'; break;
        case 31: ch='S'; break;
        case 32: ch='D'; break;
        case 33: ch='F'; break;
        case 34: ch='G'; break;
        case 35: ch='H'; break;
        case 36: ch='J'; break;
        case 37: ch='K'; break;
        case 38: ch='L'; break;
        case 39: ch='M'; break;
        case 44: ch='W'; break;
        case 45: ch='X'; break;
        case 46: ch='C'; break;
        case 47: ch='V'; break;
        case 48: ch='B'; break;
        case 49: ch='N'; break;
        case 50: ch='.'; break;

        case 2: ch='!'; break;
        case 3: ch='@'; break;
        case 4: ch='#'; break;
        case 5: ch='$'; break;
        case 6: ch='%'; break;
        case 7: ch='^'; break;
        case 8: ch='&'; break;
        case 9: ch='*'; break;
        case 10: ch='('; break;
        case 11: ch=')'; break;
        case 57: ch=' '; break;
        case 14: ch=BS; break;
        case 28: ch=EN; break;

        default: ch=0; break;
        }

    return ch;
}

void set_leds(void) {
    ecrireOctet(0x60, 0xED);
}

/* this is called by irq1 in irq.asm */
void handler_clavier(int irq) {
    keyboardLock.Take(&spinlockVar);
    
    int c = lireOctet(0x60);
    int cc;

	// Debug: Show scan code at top of screen
    draw_number(c, 60, 9, 15); 
    

    // Handle modifier keys
    switch (c) {
    case 0x2a: // Left shift press
        ControlKeys |= (KBD_SHIFT | KBD_LEFTSHIFT);
        break;
    case 0x36: // Right shift press
        ControlKeys |= (KBD_SHIFT | KBD_RIGHTSHIFT);
        break;
    case 0xaa: // Left shift release
        ControlKeys &= ~KBD_LEFTSHIFT;
        if(!(ControlKeys & KBD_RIGHTSHIFT))
            ControlKeys &= ~KBD_SHIFT;
        break;
    case 0xb6: // Right shift release
        ControlKeys &= ~KBD_RIGHTSHIFT;
        if(!(ControlKeys & KBD_LEFTSHIFT))
            ControlKeys &= ~KBD_SHIFT;
        break;
    }

    // Process key presses (not key releases which have high bit set)
    if(c < 0x80) {
        cc = writechar(c);
        
        // Process the character
        switch(cc) {
        case BS: // Backspace
            if(posBuf > 0) {
                posBuf--;
                buf[posBuf] = '\0';
                modifBuf = true; // Update this flag!
            }
            break;
            
        case EN: // Enter
            buf[posBuf] = '\n';
            if(posBuf < 255)
                posBuf++;
            buf[posBuf] = '\0';
            modifBuf = true; // Update this flag!
            break;
            
        case 0: // Unrecognized key
            break;
            
        default: // Regular character
            buf[posBuf] = cc;
            if(posBuf < 255)
                posBuf++;
            buf[posBuf] = '\0';
            modifBuf = true; // Update this flag!
            break;
        }

	    draw_number(posBuf, 90, 9, 11); // Light blue: buffer position
        draw_text(modifBuf ? "T" : "F", 120, 9, 15); // Red/magenta: modifBuf status
    }
    
    // Acknowledge the interrupt to the PIC
    ecrireOctet(0x20, 0x20);
    
    keyboardLock.Release(&spinlockVar);
}

/*
 * handler_clavier.cpp
 *
 *  Created on: 8 août 2008
 *      Author: jmenaud
 */
#include "handler_clavier.h"
#include <hal/fonctionsES.h>
#include <drivers/vga.h>  // For debug visualization
#include <drivers/KeyboardBuffer.h>

// Control key state
static int ControlKeys = 0;

int writechar(int sc) {
    int ch = 0;
    
    // Handle regular keys when SHIFT is not pressed
    if ((ControlKeys & KBD_SHIFT) == 0) {
        switch (sc) {
            // Letters
            case 16: ch = 'a'; break;
            case 17: ch = 'z'; break;
            case 18: ch = 'e'; break;
            case 19: ch = 'r'; break;
            case 20: ch = 't'; break;
            case 21: ch = 'y'; break;
            case 22: ch = 'u'; break;
            case 23: ch = 'i'; break;
            case 24: ch = 'o'; break;
            case 25: ch = 'p'; break;
            case 30: ch = 'q'; break;
            case 31: ch = 's'; break;
            case 32: ch = 'd'; break;
            case 33: ch = 'f'; break;
            case 34: ch = 'g'; break;
            case 35: ch = 'h'; break;
            case 36: ch = 'j'; break;
            case 37: ch = 'k'; break;
            case 38: ch = 'l'; break;
            case 39: ch = 'm'; break;
            case 44: ch = 'w'; break;
            case 45: ch = 'x'; break;
            case 46: ch = 'c'; break;
            case 47: ch = 'v'; break;
            case 48: ch = 'b'; break;
            case 49: ch = 'n'; break;
            
            // Numbers
            case 2: ch = '1'; break;
            case 3: ch = '2'; break;
            case 4: ch = '3'; break;
            case 5: ch = '4'; break;
            case 6: ch = '5'; break;
            case 7: ch = '6'; break;
            case 8: ch = '7'; break;
            case 9: ch = '8'; break;
            case 10: ch = '9'; break;
            case 11: ch = '0'; break;
            
            // Special keys
            case 57: ch = ' '; break;  // Space
            
            default: ch = 0; break;
        }
    }
    else {
        // Handle SHIFT + key combinations
        switch (sc) {
            // Capital letters
            case 16: ch = 'A'; break;
            case 17: ch = 'Z'; break;
            case 18: ch = 'E'; break;
            case 19: ch = 'R'; break;
            case 20: ch = 'T'; break;
            case 21: ch = 'Y'; break;
            case 22: ch = 'U'; break;
            case 23: ch = 'I'; break;
            case 24: ch = 'O'; break;
            case 25: ch = 'P'; break;
            case 30: ch = 'Q'; break;
            case 31: ch = 'S'; break;
            case 32: ch = 'D'; break;
            case 33: ch = 'F'; break;
            case 34: ch = 'G'; break;
            case 35: ch = 'H'; break;
            case 36: ch = 'J'; break;
            case 37: ch = 'K'; break;
            case 38: ch = 'L'; break;
            case 39: ch = 'M'; break;
            case 44: ch = 'W'; break;
            case 45: ch = 'X'; break;
            case 46: ch = 'C'; break;
            case 47: ch = 'V'; break;
            case 48: ch = 'B'; break;
            case 49: ch = 'N'; break;
            
            default: ch = 0; break;
        }
    }
    
    return ch;
}

void handler_clavier(int irq) {
    // Read scan code from keyboard port
    int c = lireOctet(0x60);
    
    
    // Handle key press/release
    switch (c) {
        case 0x2a: ControlKeys |= KBD_LEFTSHIFT; break;
        case 0x36: ControlKeys |= KBD_RIGHTSHIFT; break;
        
        // SHIFT release
        case 0xaa: ControlKeys &= ~KBD_LEFTSHIFT; break;
        case 0xb6: ControlKeys &= ~KBD_RIGHTSHIFT; break;
        
        default:
            if ((c & 0x80) == 0) { // Key press (not release)
                int ch = writechar(c);
                if (ch != 0) {
                    // Add to thread-safe buffer
                    KeyboardBuffer::put(ch);
                }
            }
            break;
    }
}

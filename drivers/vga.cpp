#include "vga.h"

volatile unsigned char *video = (unsigned char *)0xA0000;
/* I/O port helpers (you must have I/O privileges!) */
static inline void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline unsigned char inb(unsigned short port) {
    unsigned char val;
    asm volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

/* VGA register values for mode 0x13 (320×200, 256 colors) */
static const unsigned char vga_misc    = 0x63;
static const unsigned char vga_seq[5]  = { 0x03,0x01,0x0F,0x00,0x0E };
static const unsigned char vga_crtc[25]= {
    0x5F,0x4F,0x50,0x82,0x54,0x80,0xBF,0x1F,
    0x00,0x41,0x00,0x00,0x00,0x00,0x00,0x00,
    0x9C,0x0E,0x8F,0x28,0x1F,0x96,0xB9,0xA3,0xFF
};
static const unsigned char vga_gc[9]   = { 
    //0x00,0x00,0x10,0x0E,0x02,0x0F,0x00,0x00,0x00 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF    
};
static const unsigned char vga_attr[21]= {
    /* palette 0–15 */ 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                       0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    /* overscan, etc */ 0x41,0x00,0x0F,0x00,0x00
};


void set_vga_mode13(void) {
    int i;

    /* 1) Miscellaneous Output */
    outb(0x03C2, vga_misc);

    /* 2) Sequencer registers */
    for (i = 0; i < 5; i++) {
        outb(0x03C4, i);
        outb(0x03C5, vga_seq[i]);
    }

    /* 3) CRTC registers */
    for (i = 0; i < 25; i++) {
        outb(0x03D4, i);
        outb(0x03D5, vga_crtc[i]);
    }

    /* 4) Graphics Controller registers */
    for (i = 0; i < 9; i++) {
        outb(0x03CE, i);
        outb(0x03CF, vga_gc[i]);
    }

    /* 5) Attribute Controller registers
     *    Reset flip‑flop by reading 0x3DA before each write.
     */
    for (i = 0; i < 21; i++) {
        inb(0x03DA);
        outb(0x03C0, i);
        outb(0x03C0, vga_attr[i]);
    }

    /* Unblank display: re‑reset flip‑flop and set bit 5 */
    inb(0x03DA);
    outb(0x03C0, 0x20);
}

void clear_vga_screen(char color) {

    int i;

    for (i = 0; i < 320 * 200; i++) {
        video[i] = color;
    }
}

void plot_square(int x, int y, int size, unsigned char color) {
    int row, col;
    for (row = 0; row < size; row++) {
        int base = (y + row) * 320 + x;
        for (col = 0; col < size; col++) {
            video[base + col] = color;
        }
    }
}

void plot_palette(int x, int y, int size) {
    int row, col;
    for (row = 0; row < 16; row++) {
        int base = (y + row) * 320 + x;
        for (col = 0; col < 16; col++) {
            plot_square(x+col*size,y+row*size,size,row*16+col);
        }
    }
}


void set_palette_vga(unsigned char palette_vga[256][3]) {
    // Sélection de l'entrée 0
    outb(0x3C8, 0);
    // Envoi des 256 triplets VGA (6-bit)
    for(int i = 0; i < 256; i++) {
        outb(0x3C9, palette_vga[i][0]);
        outb(0x3C9, palette_vga[i][1]);
        outb(0x3C9, palette_vga[i][2]);
    }
}
// Draw a w×h sprite at (dstX, dstY), skipping color 255
void draw_sprite(const unsigned char* sprite,
                 int w, int h,
                 int dstX, int dstY)
{
    for (int yy = 0; yy < h; ++yy) {
        for (int xx = 0; xx < w; ++xx) {
            unsigned char c = sprite[yy * w + xx];
            if (c != 0) {
                video[(yy+dstY)*320 + (xx + dstX)] = c;
            }
        }
    }
}
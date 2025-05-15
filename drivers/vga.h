#ifndef VGA_H
#define VGA_H
extern volatile unsigned char *video;

void set_vga_mode13(void);
void set_palette_vga(unsigned char palette_vga[256][3]);

void clear_vga_screen(char color);

void copy_frame_buffer_to_video();
void clear_frame_buffer(unsigned char color);

void plot_square(int x, int y, int size, unsigned char color);
void plot_rectangle(int x, int y, int height, int width, unsigned char color);

void draw_sprite(const unsigned char* sprite, int w, int h, int dstX, int dstY);
void draw_char(char c, int x, int y, unsigned char color);
void draw_text(const char* str, int x, int y, unsigned char color);
void draw_number(unsigned int number, int x, int y, unsigned char color);

// Sprite rotation options
enum class RotationType {
    NONE,
    ROTATE_90,     // 90 degrees clockwise
    ROTATE_180,    // 180 degrees
    ROTATE_270     // 270 degrees clockwise (or 90 counter-clockwise)
};

// Draws a sprite with rotation if needed
void draw_sprite_rotated(const unsigned char* sprite, 
                         int w, int h, 
                         int dstX, int dstY, 
                         RotationType rotation = RotationType::NONE);

#endif
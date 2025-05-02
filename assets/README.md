# VGA Integration

The scripts here will permit to switch to [VGA mode](https://en.wikipedia.org/wiki/Mode_13h) (320x200, 256 colors), set a palette and plot imported sprites.

## Palette array integration

Palette is the way to set custom colors in mode. For VGA each of the 256 colors can be set to a given 0-63 value (6 bits) for RGB, thus summing up to 262144 potential colors. Using the right palette is mandatory to draw faithfully a sprite. It can be changed at runtime.

The example palette `palette_vga` in [sprite.cpp](sprite.cpp) is provided ([atari-8-bit-family-gtia.pal](atari-8-bit-family-gtia.pal) file) but of course you can change it.

```bash
python palette.py atari-8-bit-family-gtia.pal
```

Then integrated the generated array in your code, and use the function 

## Sprite array integration (with palette)

Sprites are included as unsigned char arrays, of the size of the sprite (ex: 32x32). 
The [sprite.py](sprite.py) script permits to generate the needed array (and potentially palette) from a PNG file (only). 
It is adviced to use a given palette (as each sprite will share it on the same screen). Origin colors will be quantized at best to 

```bash
python sprite.py file.png -p atari-8-bit-family-gtia.pal # if you are using this palette in your code
```

Then, include the generated array in your code and use the `draw_sprite` with it.

## Usage in code

Include [vga.h](vga.h) and [sprite.h](sprite.h)
Copy the necessary arrays for palette and sprites.

Use the following code (for instance on key pressed): 
```cpp
set_vga_mode13(); // set VGA mode
set_palette_vga(palette_vga); // set to given palette
clear_vga_screen(0); // put the color 0 on each pixel
plot_square(50, 50, 25, 4); // plot a square of 25 width at 50,50 of color 4
draw_sprite(sprite_door_data, 32, 32, 100,100); // draw the 32x32 sprite at 100,100
```

### "Transparent" color

It is assumed in `draw_sprite` that the transparent color (not drawed) is the 255. You can change this at ease. Don't forget to take that into account in your sprites!

## Usage with Sextant

To see your program in VGA mode, you need to launch your program in "graphics mode" and connect using a VNC client on the 5900 port. The principle has been tested successfully with TightVNC (Windows) and Remmina (Linux).

## Additional resources

[Palettes](https://lospec.com/palette-list)
[Pixel editor](https://apps.lospec.com/pixel-editor)
[Free sprites](https://itch.io/game-assets/free)
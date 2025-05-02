#!/usr/bin/env python3
import sys
import re

def parse_jasc(lines):
    # saute les 3 premières lignes, puis récupère 256 lignes R G B
    data = [list(map(int, l.split())) for l in lines[3:3+256]]
    return data

def parse_gpl(lines):
    data = []
    for l in lines:
        l = l.strip()
        if not l or l.startswith('#') or re.match(r'GIMP Palette', l):
            continue
        parts = l.split()
        if len(parts) >= 3:
            data.append(list(map(int, parts[:3])))
    return data[:256]

def rgb_to_vga(r, g, b):
    # passage 0–255 → 0–63 (VGA 6 bits)
    return r * 63 // 255, g * 63 // 255, b * 63 // 255

def main(path):
    with open(path, 'r') as f:
        lines = f.readlines()
    if lines[0].startswith('JASC'):
        pal = parse_jasc(lines)
    elif lines[0].startswith('GIMP'):
        pal = parse_gpl(lines)
    else:
        # .act binaire ?
        with open(path, 'rb') as fb:
            raw = fb.read(768)
        pal = [list(raw[i:i+3]) for i in range(0, 768, 3)]
    # conversion VGA
    vga = [rgb_to_vga(r, g, b) for r, g, b in pal]

    # génération du code C
    print("/* Généré par convert_palette.py */")
    print("unsigned char palette_vga[256][3] = {")
    for r, g, b in vga:
        print(f"    {{{r}, {g}, {b}}},")
    print("};\n")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: convert_palette.py <palette.pal|.gpl|.act>")
        sys.exit(1)
    main(sys.argv[1])
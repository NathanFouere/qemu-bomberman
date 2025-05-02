#!/usr/bin/env python3
"""
png_to_h_header.py: Génère un fichier d'en-tête C avec les définitions pour chaque sprite d'un dossier PNG,
avec les tailles des sprites, les palettes VGA et les déclarations de tableaux pour chaque sprite.

Usage:
    python3 png_to_h_header.py [-p PALETTE_FILE] <dossier_images>

Génère un fichier .h avec :
  - #define SPRITE_WIDTH  <width>
  - #define SPRITE_HEIGHT <height>
  - extern unsigned char sprite_data_<nom_fichier>[(width*height)];
  - extern unsigned char palette_vga[256][3];
"""
import sys
import re
import os
from PIL import Image
import argparse

def parse_jasc_pal(path):
    """Parse un fichier JASC-PAL (.pal) en liste de 256 tuples RGB."""
    with open(path, 'r') as f:
        lines = f.read().splitlines()
    # Format: JASC-PAL / version / 256 / R G B...
    header = lines[:3]
    count = int(header[2])
    data = []
    for line in lines[3:3+count]:
        parts = line.strip().split()
        if len(parts) >= 3:
            data.append(tuple(map(int, parts[:3])))
    # Complète si <256
    data += [(0,0,0)] * max(0, 256 - len(data))
    return data[:256]


def parse_gimp_gpl(path):
    """Parse un fichier GIMP-GPL (.gpl) en liste de 256 tuples RGB."""
    data = []
    with open(path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#') or re.match(r'GIMP Palette', line) or line.startswith('Name:') or line.startswith('Columns:'):
                continue
            parts = line.split()
            if len(parts) >= 3:
                data.append(tuple(map(int, parts[:3])))
    data += [(0,0,0)] * max(0, 256 - len(data))
    return data[:256]


def rgb_to_vga(r, g, b):
    """Convertit RGB 8-bit (0–255) en VGA 6-bit (0–63)."""
    return (r * 63) // 255, (g * 63) // 255, (b * 63) // 255


def build_palette_image(palette):
    """Construit une image PIL mode 'P' 1x256 avec la palette donnée."""
    pal_img = Image.new('P', (1, 1))
    flat = []
    for (r,g,b) in palette:
        flat.extend([r, g, b])
    pal_img.putpalette(flat)
    return pal_img


def generate_header(directory, external_palette=None, output_file="sprites.tmp.h"):
    """Génère un fichier d'en-tête .h pour tous les PNG dans un dossier."""
    with open(output_file, 'w') as f:
        f.write("// Fichier d'en-tête généré par png_to_h_header.py\n")
        f.write("#ifndef SPRITES_H\n#define SPRITES_H\n\n")
        
        for filename in os.listdir(directory):
            if filename.endswith('.png'):
                file_path = os.path.join(directory, filename)
                # Utilisation du nom de fichier sans l'extension .png comme nom de la fonction
                array_name = f"{os.path.splitext(filename)[0]}"
                
                # Ouvrir l'image et récupérer ses dimensions
                im = Image.open(file_path)
                w, h = im.size
                
                # Définir les tailles des sprites et déclarations externes
                # f.write(f"#define {array_name.upper()}_WIDTH  {w}\n")
                # f.write(f"#define {array_name.upper()}_HEIGHT {h}\n")
                f.write(f"extern unsigned char {array_name}[SPRITE_WIDTH*SPRITE_WIDTH];\n")
                
                # Ajouter une déclaration de palette VGA
                #f.write(f"extern unsigned char palette_vga[256][3];\n\n")

        f.write("#endif // SPRITES_H\n")

    print(f"Le fichier d'en-tête a été généré sous {output_file}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Générer un fichier d'en-tête C pour les sprites dans un dossier PNG.")
    parser.add_argument('-p', '--palette', help="Fichier de palette externe (.pal ou .gpl)")
    parser.add_argument('directory', help="Dossier contenant les images PNG à convertir")
    parser.add_argument('-o', '--output', default="sprites.h", help="Nom du fichier de sortie pour le header (par défaut: sprites.h)")
    args = parser.parse_args()
    
    generate_header(args.directory, external_palette=args.palette, output_file=args.output)

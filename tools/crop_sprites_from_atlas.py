"""
crop_sprites_from_atlas.py

This script crops the sprites from a given atlas and saves the output images in the provided folder.

--atlas : atlas file to be cropped
--prefix : the prefix of the images that will be cropped from the atlas
--num_rows : how many rows of sprites in the atlas
--num_cols : how many columns of sprites in the atlas
--img_folder : folder where the cropped images will be saved

Usage:

python crop_sprites_from_atlas.py --atlas=./my_folder/atlas.png 
    --prefix=hero_ --num_rows=10 --num_cols=5 --img_folder=./my_folder

Example:

    python crop_sprites_from_atlas.py --atlas=sprites/Coin.png --prefix=coin --num_rows=1 --num_cols=8 --img_folder=sprites/coin/

Requirements:
This script requires the library pillow to be installed

"""

import argparse
import glob
import json
import logging
import math
import sys

from pathlib import Path
from PIL import Image

def crop(atlas, rows, cols, prefix, output_folder):
    im = Image.open(atlas)
    w, h = im.size
    
    sw = math.ceil(w/cols)
    sh = math.ceil(h/rows)
    sn = 0
    for ridx in range(rows):
        for cidx in range(cols):
            x = cidx * sw
            y = ridx * sh
            sprite = im.crop((x,y,x + sw,y + sh)) 
            sprite_filename = f'{output_folder}{prefix}_{sn}.png'
            sprite.save(sprite_filename)
            logging.info(f'generated: {sprite_filename}')
            sn += 1

def main():
    logging.basicConfig(level=logging.INFO,
                        format='>>> %(process)d %(filename)s %(levelname)s %(message)s',
                        stream=sys.stdout)

    # parse command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('--img_folder', dest='img_folder', help='folder where the cropped images will be saved')
    parser.add_argument('--atlas', dest='atlas', help='atlas file to be cropped')
    parser.add_argument('--prefix', dest='prefix', help='the prefix of the images that will be cropped from the atlas')
    parser.add_argument('--num_rows', dest='num_rows', type=int, help='number of rows of sprites in the atlas')
    parser.add_argument('--num_cols', dest='num_cols', type=int, help='number of columns of sprites in the atlas')
    parser.add_argument('--verbose', dest='verbose', action='store_true', default=False)

    opts, unknown = parser.parse_known_args()

    if opts.verbose:
        logging.getLogger().setLevel(logging.DEBUG)
        logging.debug('setting log level to debug')

    logging.info(f'Saving images to folder "{opts.img_folder}". Output files will be "{opts.prefix}-x.png".')

    img_folder_full_path = Path(opts.img_folder).resolve()

    logging.debug(f'Full path of the folder is: {img_folder_full_path}')

    crop(opts.atlas, opts.num_rows, opts.num_cols, opts.prefix, opts.img_folder)

    logging.info(f'Job completed.')

if __name__ == '__main__':
    main()
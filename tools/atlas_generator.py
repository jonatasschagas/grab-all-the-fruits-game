"""
atlas_generator.py

This script packs the images found in the given directory into one atlas image. It also generates
a json file with the mapping of the images in the atlas image.

Usage:

python atlas_generator.py --img_folder=./my_folder/ --name=my_character

Example:
    python atlas_generator.py --img_folder=../Art/Kobold/Individual\ Sprites/  --name=kobold  

Requirements:
This script requires the library pillow to be installed

"""

import argparse
import glob
import json
import logging
import sys

from pathlib import Path
from PIL import Image

ATLAS_WIDTH_THRESHOLD = 640

def generate_atlas(list_of_images, output_img, img_format, output_config_file, resize, rw, rh):

    logging.info(f'Working with {len(list_of_images)} images.')
    
    # estimating the dimensions of the output atlas image
    cur_x = 0
    cur_y = 0
    max_height = 0
    max_width = 0
    for img_path in list_of_images:
        img = Image.open(img_path)
        img_w = img.size[0]
        img_h = img.size[1]
        logging.debug(f'w: {img_w} : h: {img_h}')
        if resize:
            cur_x += rw
        else:
            cur_x += img_w
        if not resize and img_h > max_height:
            max_height = img_h
        elif resize:
            max_height = rh
        if cur_x >= ATLAS_WIDTH_THRESHOLD:
            if cur_x > max_width:
                max_width = cur_x
            cur_x = 0
            cur_y += max_height
            max_height = 0
            logging.debug(f'next line')
    cur_y += max_height

    if cur_x > max_width:
        max_width = cur_x

    logging.info(f'Atlas image will be: {max_width} x {cur_y}.')
    atlas_img = Image.new('RGBA', (max_width, cur_y))
    
    cur_x = 0
    cur_y = 0
    max_height = 0
    max_width = 0

    atlas_config = []

    for img_path in list_of_images:
        img = Image.open(img_path)
        img_w = img.size[0]
        img_h = img.size[1]
        
        # appending the image to the atlas img
        if resize:
            img = img.resize((rw,rh), Image.ANTIALIAS)
            atlas_img.paste(img, (cur_x, cur_y))
        else:
            atlas_img.paste(img, (cur_x, cur_y))
        
        # appending to the configuration
        image_name = img_path.split('/')[-1:][0]
        if resize:
            atlas_config.append({
                'name': image_name,
                'x': cur_x,
                'y': cur_y,
                'width': rw,
                'height': rh
            })
        else:
            atlas_config.append({
                'name': image_name,
                'x': cur_x,
                'y': cur_y,
                'width': img_w,
                'height': img_h
            })
        
        if resize:
            cur_x += rw 
        else:
            cur_x += img_w
        
        if not resize and img_h > max_height:
            max_height = img_h
        elif resize:
            max_height = rh
        if cur_x >= ATLAS_WIDTH_THRESHOLD:
            if cur_x > max_width:
                max_width = cur_x
            cur_x = 0
            cur_y += max_height
            max_height = 0
    
    atlas_img.save(output_img, img_format)

    logging.info(f'Atlas image generated: {output_img}')

    with open(output_config_file, 'w') as config_outfile:
        json.dump(atlas_config, config_outfile, indent=4)

    logging.info(f'Atlas config generated: {output_config_file}')

def read_images_from_path(path):
    image_types = ['png', 'jpg', 'jpeg']
    imgs = []
    for img_type in image_types:
        imgs += glob.glob(f'{path}/*.{img_type}')

    imgs.sort()

    nl = '\n'   
    logging.debug(f'Found:{nl} {("," + nl).join(imgs)}')

    return imgs

def main():
    logging.basicConfig(level=logging.INFO,
                        format='>>> %(process)d %(filename)s %(levelname)s %(message)s',
                        stream=sys.stdout)

    # parse command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('--img_folder', dest='img_folder', help='Folder in which the images are located.')
    parser.add_argument('--name', dest='name', help='Name of the output files.')
    parser.add_argument('--verbose', dest='verbose', action='store_true', default=False)
    parser.add_argument('--resize', dest='resize', help='Resize the images to a certain size', default=False)
    parser.add_argument('--rw', dest='rw', help='Width to resize', default=0, type=int)
    parser.add_argument('--rh', dest='rh', help='Height to resize', default=0, type=int)

    opts, unknown = parser.parse_known_args()

    if opts.img_folder is None:
        raise ValueError('Please provide a valid folder, using the parameter: --img_folder')

    if opts.name is None:
        raise ValueError('Please provide a valid name for the output files.')

    if opts.verbose:
        logging.getLogger().setLevel(logging.DEBUG)
        logging.debug('setting log level to debug')

    logging.info(f'Reading images from folder "{opts.img_folder}". Output files will be "{opts.name}-atlas.png" and "{opts.name}.json".')

    img_folder_full_path = Path(opts.img_folder).resolve()

    logging.debug(f'Full path of the folder is: {img_folder_full_path}')

    imgs = read_images_from_path(img_folder_full_path)

    output_file_format = 'png'
    atlas_output_file = f'{img_folder_full_path}/{opts.name}.{output_file_format}'
    atlas_config_output_file = f'{img_folder_full_path}/{opts.name}.json'

    generate_atlas(imgs, atlas_output_file, output_file_format, atlas_config_output_file, opts.resize, opts.rw, opts.rh)
    
    logging.info(f'Job completed.')

if __name__ == '__main__':
    main()
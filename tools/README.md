## Tools

#### virtual environment
Use a virtual environment to install the dependencies of the scripts:
```
python3 -m pip install --user virtualenv
cd tools
python3 -m venv tools
source tools/bin/activate
pip3 install -r requirements.txt
```

#### crop_sprites_from_atlas.py

This script crops sprites from a spritesheet into individual sprite files. See comments at the top of the script to see how to use it.

#### atlas_generator.py

Combines all the sprites from a given directory into one big atlas file. It also generates a JSON file with all the mappings of each image in the atlas. See comments at the top of the script to see how to use it.


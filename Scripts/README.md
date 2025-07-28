## unpacker.py

Unpack assets from the asset packing file   

usage: unpacker.py [-h] [-o OUTPUT] input_file   

## spt_to_png_3.py

Convert .spt images to .png ones   

usage: spt_to_png_3.py [-h] [-o OUTPUT] input_dir/input_file

NOTE: each pixel's channel's max values are 240, which is how EKA2L1 seems to display all the sprites anyways (the NOKIA logo during game boot, for example, is rgb(240, 240, 240), the decoded SNAKES logo's colors perfectly match what the emulator displays), though I am unsure as to how to treat the alpha channel. You can edit the script or the images if you want 100% opacity, as of right now all images it produces are never 100% opaque, max alpha is 240 and it never goes up to 255.

## png_to_spt.py

Convert .png images to .spt ones

usage: spt_to_png_3.py [-h] [-o OUTPUT] input_dir/input_file

NOTE: multi-image spt files require proper filename structure to be encoded. Example: ```image[[1;2]]__frame0.png```, where the square brackets denote the X and Y offsets of the image sequence and ```__frameX``` denotes the index of the image in the sequence.

## bix_converter.py

Convert .bix models to .gltf ones and back, too!

usage: bix_converter.py --bix-to-gltf/--gltf-to-bix <input_path> [output_path]

## PAKC_decryptor

A reimplementation of the Blowfish cipher decryption based on the IDA PRO decompilation of Snakes. Should turn encrypted ```.pakc``` files into readable ```.pak``` archives. More information in the related folder.

## Savedata editing

The XoR cipher along with MD5 hash validation that have been ripped from the game, allowing you to decode and edit the raw binary data of the savefiles.

## unpacker.py

Unpack assets from the asset packing file   

usage: unpacker.py [-h] [-o OUTPUT] input_file   

## spt_converter_2.py

Convert .spt images to .png ones   

usage: spt_converter_2.py [-h] [-o OUTPUT] input_dir   

NOTE: each pixel's channel's max values are 240, which is how EKA2L1 seems to display all the sprites anyways (the NOKIA logo during game boot, for example, is rgb(240, 240, 240), the decoded SNAKES logo's colors perfectly match what the emulator displays), though I am unsure as to how to treat the alpha channel. You can edit the script or the images if you want 100% opacity, as of right now all images it produces are never 100% opaque, max alpha is 240 and it never goes up to 255.

## PAKC_decryptor

A reimplementation of the Blowfish cipher based on the IDA PRO decompilation of Snakes. 
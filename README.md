# Snakes (NGage game) Assets Extraction Guide

### What you'll need:
1. A ```.sis``` of Snakes below version 0.6.X.X (Explanation will follow, info on versions is fuzzy)
2. A working setup of [EKA2L1](https://github.com/EKA2L1/EKA2L1) (to extract the .PAK files)
3. [Offzip](http://aluigi.altervista.org/search.php?src=offzip) to unpack the .PAK files

### How to extract asset packing files PAK files:
1. Install the ```.sis``` you have into EKA2L1.
2. Locate the .PAK files (Mine were located over at ```\[PATH TO YOUR EKA2L1 INSTALL\]\data\drives\e\System\Apps\6R45)
   Snakes has 5 .PAK files, numbered 6R45-ZZ01 through 6R45-ZZ05
3. Run offzip on each file to extract the assets in some proprietary (?) format that will now need to be unpacked

### What are these files you just extracted?
Offzip by default names them 00000031.dat (rarely they have a different name). For intents of this guide, I'll refer to them as the ```Asset packing files```.   
They contain a number of game assets, each one having an entry in the header of the file. Headers are 32 bytes long. 

![heres a pic of the header](imgs/asset%20packing%20header.png)

First word contains the length of the file name. Second word is the offset from the beginninng of the file at which the file name begins. Fourth and fifth word follow the same logic as described above, but they point to the contents of the file itself along with its length. Last 2 words are reserves for some sort of a numbering system.

You can find the exact length of the header by looking at the offset of the filename of the first header entry. It points to the beginning of the section that holds the filenames, which is where the header ends.

Now simply parse the ```Asset packing files``` and you will have each asset to play with. Let's take a look at some of them.

## ```.adp``` files
These are sound files, 8KHz sample rate, VOX ADPCM(?) encoding. You can play them through [Audacity](https://www.audacityteam.org/) by importing each as a raw file with the settings mentioned before. Here are the sounds [uploaded to YouTube](https://www.youtube.com/watch?v=SK5fnwmWgrs).

## ```.spt``` files
These are the in-game sprites. Each ```.spt``` file can conntain multiple images, this is denoted by the first byte of the first word. ```x02``` is singe-image, ```x06``` is multi-image. Second word correspongs to the number of images contained within the file. Bytes at offsets ```x08``` and ```x0C``` are image width and image height respectively. These seem to remain constant for all images in the file. After this, the byte at ```x0D``` in single-image files and at ```x15``` in multi-image files is the number of colors that will be defined in the file. After that, pairs of bytes describe 4-bit ARGB colors. The word that follows the color definition section is in big-endian and is the length of the RLE-encoded pixel data.

![pic of a single-image .spt file header](imgs/spt%20format.png)
![pic of a multi-image .spt file header](imgs/spt%20multi%20format.png)

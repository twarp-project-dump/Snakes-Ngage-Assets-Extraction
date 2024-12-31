# Snakes (NGage game) Assets Extraction Guide

### Repo structure
- [Scripts](/Scripts) will contain ```.py``` scripts that can be used to extract assets from ```Asset packing files``` as well as instructions (the scripts not yet here sorry)
- [Assets](/Assets) will contain the raw extracted assets from each version of the game that I could find (not yet here either sorry)
- [Decoded Images](/Decoded%20Images) will contain the decoded sprites from each game version that I could find

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
These are the in-game sprites. Each ```.spt``` file can conntain multiple images, this is denoted by the first byte of the first word. ```x02``` is singe-image, ```x06``` is multi-image. Second word correspongs to the number of images contained within the file. Bytes at offsets ```x08``` and ```x0C``` are image width and image height respectively. These seem to remain constant for all images in the file. After this, the byte at ```x0D``` in single-image files and at ```x15``` in multi-image files is the number of colors that will be defined in the file. After that, pairs of bytes describe 4-bit ARGB colors. The word that follows the color definition section is in big-endian and is the length of the RLE pixel data. There are certain exceptions (marked on the image as dual-color) and will be explained in the next section. The sections that are not highlighted have an unknown purpose. The additional 2 words in multi-image ```.spt``` files have an unknown purpose and they are not always equal to 0. (They could be related to animated images and be the frame delay values that the game uses, but this is completely untested.)

![pic of a single-image .spt file header](imgs/spt%20format.png)
![pic of a multi-image .spt file header](imgs/spt%20multi%20format.png)

### The RLE (run length encoding)

The pixel data works as follows: it always starts with a color. If the image has 10 colors defined, then it could be any one of those (colors start at 0) (however, when working on the asstes from the prerelease of the game that is up on archive.org, evolver.spt had a color that was outside of the range. Image seemed fine if the offending color had a modulo operator applied, where the color was being divided by the length of the colors defined in the file). If the color bit had its uppermost bit set, then it has its run length encoded. The byte that follows an RLE color is the run length, however, if it is above 128, then the run length is the value of said byte minus 1.

If there are less than 16 colors defined in the image (details fuzzy on this), then the RLE color may have 4-bit RLE applied instead. The following byte will be the next color, rather than the run length. The color byte in this situation is split in two, the uppermost 4 bits are the RLE data and the lowermost 4 are the color data (as such, the 16 color limit is an educated guess). To get the run length of the color, you need to ignore the uppermost bit and look at the next 3 (for example, ```xA1``` from the first ```.spt``` header example image is 10100001 in binary form. Then 1010 is the RLE data and 0001 is the color data. This means that its run length is 010 in binary, or just 2, and its color is color 1)

If there are multiple images in the ```.spt``` file, then the next RLE chunk length word is located at the end of the previous RLE chunk end.

## ```.bix``` files
These seem to be the 3d models used in the game. I have not tried to parse these yet, so any help would be appreciated.

## ```.lsc``` files
Barely researched, seem to define the level triggers, pickups or paths.

## ```.dat``` files
Seem to be the files that contain level geometry or other info about the levels.

## ```.txt``` files
These include the slight dev commentary, list of tracks that play in each set of level and the sets of levels themselves. There are mentions of "groovetracker" scripts, which is apparently how the game decides which music to play. You can look at those scripts yourself by opening the files.

The prerelease did not have the ```.lsc``` files in it and instead had text files that described the paths and triggers and also contained short dev comments on the levels and level spawns.

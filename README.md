# Snakes (NGage game) Assets Extraction Guide

### Repo structure
- [Scripts](/Scripts) contain ```.py``` scripts and other software that can be used to extract the assets from the game
- [Assets](/Assets) contain the raw extracted assets from each version of the game that I can get my hands on
- [Decoded Images](/Decoded%20Images) contain in-game sprites (```.spt``` files) converted to ```.png``` files
- [Decoded OST](/Decoded%20OST) contains all the sounds from Snake60 version 0.5.1.4 in ```.wav``` format

### What you'll need to extract assets yourself:
1. A ```.sis``` of Snakes (version 0.6.0.4 A3 or below recommended)
2. A working [EKA2L1](https://github.com/EKA2L1/EKA2L1) setup (to extract the .PAK files from the game) (...though any other method of unpacking ```.sis``` files should work, too.)
3. [Offzip](http://aluigi.altervista.org/search.php?src=offzip) to extract data from the .PAK files

If you wish to forgo installing EKA2L1, then https://github.com/1upus/N-Gage_stuff/tree/master has a number of programs that can extract files from the ```.sis``` as well as a script for [QuickBMS](https://aluigi.altervista.org/quickbms.htm) that allows you to extract the data from the ```.dat``` files that the game uses.

### How to extract the asset packing files from ```.PAK``` files:
1. Install the ```.sis``` you have into EKA2L1
2. Locate the ```.PAK``` files (Mine were located over at ```\[PATH TO YOUR EKA2L1 INSTALL]\data\drives\e\System\Apps\6R45```)   
   Snakes has 5 ```.PAK``` files, numbered 6R45-ZZ01 through 6R45-ZZ05   
   Each ```.PAK``` file should start with the magic number ```F7 D3 1F 10```   
   !! If you end up with ```.pakc``` files, then you have a version of the game that has the assets _encrypted_. The encryption is an implementation of blowfish, more on that later.
3. Run [offzip](http://aluigi.altervista.org/search.php?src=offzip) on each file to extract the assets

### What are these files that you just extracted?
Offzip (usually) names them 00000031.dat (rarely, they do have a different name, unsure as to why). For the intents of this guide thing, I'll refer to them as the ```Asset packing files```.   
Each ```asset packing file``` contains — well — in-game assets. For each asset there is a 32-byte-long entry in the header of the file.

![heres a pic of the header](imgs/asset%20packing%20header.png)

First word of the header entry contains the length of the filename. Second word is the offset from the beginning of the file at which the file name begins. Fourth and fifth word point to the contents of the file itself along with the content's length, following the same logic as described above. Last 2 words are reserved for some sort of a numbering system.

You can find the exact length of the header by looking at the location of the first asset's filename, as it should be located exactly where the header ends.

This information should be enough to extract each asset from the ```asset packing file```, so this repo offers [a Python script](/Scripts/unpacker.py) that can extract these files for you.

## ```.adp``` files
These are the sound files. They are stored as IMA ADPCM at 8KHz sample rate. You can convert them to other formats through the use of [SoX](https://sourceforge.net/projects/sox/). They are available in an already-converted state both [in this repo](/Decoded%20OST) in ```.wav``` format, as well as on [youtube](https://youtu.be/4M8aX67pUe4).   

Note: From my testing, it would seem that replacing these files with anything longer than the original filesize causes the game to crash upon boot. Never tried to edit the SFX, nor did I figure out why the game crashes by looking at the code through IDA PRO.

## ```.spt``` files
These are the in-game sprites. Each ```.spt``` file can contain multiple images, which is determined by the first byte of the first word of the file. ```x02``` denotes a singe-image ```.spt```, while ```x06``` corresponds to one that contains multiple images. Second word corresponds to the number of images contained within the file. Bytes at offsets ```x08``` and ```x0C``` are the sprite's width and height respectively, these seem to remain constant for all images contained in multi-image ```.spt``` files. The byte over at ```x0D``` in single-image files and at ```x15``` in multi-image files is the number of colors defined for the sprite and it is followed by pairs of bytes describing the 4-bit ARGB colors. The word that follows the color definition section stores the length of the RLE pixel data in big-endian, the RLE data itself then follows, which will be explained in the next paragraph. The sections that are not highlighted have an unknown purpose. The additional 2 words in multi-image ```.spt``` files are the image sequence's X and Y offsets respectively. Unsure as to why that's only a feature of multi-image spt files, but it is.

![pic of a single-image .spt file header](imgs/spt%20format.png)
![pic of a multi-image .spt file header](imgs/spt%20multi%20format.png)

### The RLE (run length encoding)
The pixel data works as follows: it always starts with a color. For example, if the image has 10 colors defined, then the first byte could be any one of those. Color indexing starts with 0. (A note about the indexing: when working with the assets from the prerelease of the game, which is up on archive.org, the sprite ```evolver.spt``` had a single color set in the RLE data area that was outside of the defined range. A modulo operator applied to the offending colors, dividing them by the number of the defined colors, seemed to have resolved the issue). If the color byte has its uppermost bit set, then its run length is encoded. The byte that follows an RLEd color is the run length itself. However, if the run length is above 128, then two bytes are used to encode it instead. In that case, the run length is equal to the sum of the first run length byte and the value of the next one, but multiplied by 128.

If there are less than 16 colors defined in the image (details fuzzy on this), the RLE color data may have 4-bit RLE applied instead. An example is shown in the image by the singular color byte highlighted by both brown and cyan. A 4-bit RLEd color byte is not followed by its run length, rather, it has the length stored in the uppermost 4 bits, the lowermost 4 being the color data itself (as such, the 16 color limit is an educated guess, based on how many colors you could encode in 4 bits). To get the actual run length of the color, you need to ignore the uppermost bit and look at the last 3 _(so, maybe it's 3-bit RLE?)_. For example, ```xA1``` from the first ```.spt``` header example image is ```10100001``` in binary form. Then ```1010``` is the RLE data and ```0001``` is the color data. This means that its run length is ```010``` in binary, or just 2, and its color is index 1.

If there are multiple images in the ```.spt``` file, then the next RLE chunk length word is located at the end of the previous RLE chunk.

## ```.bix``` files
These seem to be the 3d models used in the game. They only contain the vertex and face data, no colors or textures or anything.

Each file starts with `01 00 00 00`, followed by a word containing the number of animation frames the file has (i.e. just vertex groups) and then by the number of vertices in the model, also a word. At offset 0x0C, each vertices' coordinates are stored, one word for each of the axes. At 0x0C + ```<number of vertices> x <number of animation frames> x 3``` is the word denoting the number of faces the model has, the data for each face's vertices then follows.

## ```.lsc``` files
These are called ```level scripts``` by ```levelDefs.txt``` and are used to define the level triggers, pickups or paths. They resemble the level trigger and pickup definition ```.txt``` files used in the prerelease.   

Each file usually begins with ```n:```, which points to the level's name in the Snakes' resource files. For example, ```n: 0x5dcb7165``` gets the resource file with the UID ```0x5dcb7``` and then gets the ```0x165```th entrym using it as the level name. If the level script lacks an entry of ```n: ```, then the level's .dat file's name is used as the level name.   

## ```.dat``` files
Seem to be the files that contain level geometry and other info about the levels.   

## ```.txt``` files
These include dev commentary, the list of tracks that play in each set of levels and said sets of levels themselves.   

The game uses ```Groovetracker``` scripts to decide which music files to play and where, they are also stored as ```.txt``` files.

## Game savedata

These are encrypted by an XoR cipher and they contain an MD5 hash at the end to ensure data integrity, I guess. This repo [contains two C programs](Scripts/Savedata%20editing) to decode them for editing and reencode them back into a state that the game accepts.

### sts.bin

This is the "Continue" savedata, used for when you decide to continue the game on the level you left off, keeping track of your lives, score and NOKIA/SNAKES letter collection.

It always starts with ```FF FF FF FF```

### sno.bin

This is the general savedata, including the settings, hi-scores and mastery status.

It always starts with ```05 00 00 00```, followed by hi-score data. Each entry seemingly stores the score itself, then the level number, then the time spent in seconds, each of these taking up 56 bytes. The game displays 5 hi-score entries.   

Near the end of the file are the audio settings, followed by the game mastery flag and the unlocked level count. The end of the file has the data related to the controls, then the MD5 hash. It would seem as if the file continuously has a new MD5 hash appended to it, which could be a bug in the version of the game I was testing or an intentional measure.   

I don't have the exact offsets of anything for either of these files, testing got a bit too tedious and looking at the IDA PRO decompilation did not help me much.   

## The Asset Encryption Issue
If you had extracted the ```Asset Packing Files``` from a version of the game too new, you will see that the filename is different (```.pakc``` rather than ```.pak```) and you will find yourself unable to extract the assets with [offzip](http://aluigi.altervista.org/search.php?src=offzip).

For whatever reason, the newer versions of the game have the ```Asset Packing Files``` **encrypted using [Blowfish cipher](https://en.wikipedia.org/wiki/Blowfish_(cipher))**. It seems as if they are encrypted in ECB mode, each archive using its own key, and the keys stored over at address ```00081D90``` in the ```.text``` section of the executable (at least when I analyzed the ```.exe``` in IDA PRO, I am not very experienced with this).

You can use a standard blowfish implementation to decrypt a ```.pakc``` file, though the first word will will be garbage data and it should be discarded. This repo contains [a reimplementation of the cipher decryption based on the disassembly produced by IDA PRO](Scripts/PAKC_decryptor) that can also be used to decrypt each archive, the keys being already baked into the executable.

Here are the keys for each of the five ```.pakc``` archives, in plaintext, for if you'd want to decrypt the archives yourself:

- ```asefcsee```

- ```sddfcer4```

- ```3434frdc```

- ```fvbtgrsf```

- ```34fgrfgf```

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
What are they?

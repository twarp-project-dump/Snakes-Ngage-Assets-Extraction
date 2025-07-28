The savedata is stored at ```\data\drives\c\system\data```

```sns.bin``` is the general savedata, including the settings, hi-scores and mastery status, whereas ```sts.bin``` is the "Continue" savedata, used for when you decide to continue the game on the level you left off, keeping track of your lives, score and NOKIA/SNAKES letter collection.

./savedata_decoder.exe <sno.bin/sts.bin file>

./savedata_encoder.exe <input.decrypted> <output.bin>
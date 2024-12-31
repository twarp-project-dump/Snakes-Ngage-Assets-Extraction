import struct
import codecs

path_to_file = ""

def unpack_speechpack(file_path):
    with open(file_path, 'rb') as f:
        first_fname = f.read(8)
        header_len = struct.unpack('<i', first_fname[4:8])[0]
        print(header_len)
        f.seek(0)
        header = f.read(header_len)

        header_entry = {
            "fname_len": 0,
            "fname_off": 0,
            "sound_len": 0,
            "sound_off": 0,
        }

        offsets = []
        for i in range(0, header_len, 32):
            new_entry = header_entry.copy()

            new_entry["fname_len"] = struct.unpack('<i', header[i:i+4])[0]
            new_entry["fname_off"] = struct.unpack('<i', header[i+4:i+8])[0]
            new_entry["sound_len"] = struct.unpack('<i', header[i+12:i+16])[0]
            new_entry["sound_off"] = struct.unpack('<i', header[i+16:i+20])[0]
            offsets.append(new_entry)
        
        for i, offset in enumerate(offsets):
            f.seek(offset["fname_off"])
            data = f.read(offset["fname_len"])

            sound_name = data.decode('unicode_escape')[:-1]

            print(sound_name)
            
            f.seek(offset["sound_off"])
            data = f.read(offset["sound_len"])
            
            with open(f'{path_to_file}\\{sound_name}', 'wb') as audio_file:
                audio_file.write(data)
            print(f'{sound_name} written')

unpack_speechpack(f'{path_to_file}\\5.dat')

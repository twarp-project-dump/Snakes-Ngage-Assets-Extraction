import numpy as np
from PIL import Image
import struct
from matplotlib import pyplot as plt
import matplotlib.animation as animation
import sys
from os import listdir
from os.path import isfile, join

path_to_folder = ""

path_to_save_folder = ""

class EncodedImage:
    def __init__(self, offset, length):
        self.offset = offset
        self.length = length

def read_spt_file(spt_path_ : str, img_name : str):
    print(f"Currently reading {img_name}")
    data = np.fromfile(spt_path_, dtype='B', count=-1)

    if len(data) == 0:
        print("its empty")
        return

    spt_type = int(data[0])

    images_stored = int(data[4])
    print(f"likely {images_stored} images detected")

    image_x = int(data[8])
    image_y = int(data[12])

    color_offset = 13

    color_table = {}

    binner = lambda x: bin(x)[2:]
    padder = lambda x: str(x).zfill(8)
    vec_binner = np.vectorize(binner)
    vec_padder = np.vectorize(padder)

    output_images = []

    if spt_type == 2:
        color_array_len = int(data[color_offset])
    else:
        color_offset = color_offset + 8
        color_array_len = int(data[color_offset])

    #print(f"{color_array_len} colors")

    def parse_color(parse_at:int):
        bytes_two = data[parse_at:parse_at+2]
        if len(bytes_two) != 2:
            print("error processing colors")
            return
        bytes_two_binned = vec_padder(vec_binner(bytes_two))
        tr = int(bytes_two_binned[0][0:4], 2)*16
        r = int(bytes_two_binned[0][4:8], 2)*16
        g = int(bytes_two_binned[1][0:4], 2)*16
        b = int(bytes_two_binned[1][4:8], 2)*16
        return np.array([r, g, b, tr], dtype=np.uint8)

    def parse_color_chunk(parse_at:int, parse_until:int):
        color_id = 0
        i_ = parse_at
        while i_ < parse_at+parse_until:
            #print(i_)
            color_table[color_id] = parse_color(i_)
            i_ += 2
            color_id+=1

    parse_color_chunk(color_offset + 1, color_array_len*2)

    #print(color_table)

    init_offset = color_array_len*2 + color_offset + 1

    #print(f"getting compressed chunk length at {init_offset}")

    while data[init_offset+3] == 0:
        init_offset += 4

    def get_chunk_length(at_where:int):
        #print(f"reading chunk len at {at_where}")
        chunk_len_word = data[at_where:at_where+4]
        #print(chunk_len_word)
        bin_array = vec_binner(chunk_len_word)
        padded_array = vec_padder(bin_array)
        chunk_len = int( ''.join(padded_array), 2 )
        return chunk_len

    current_read_offset = init_offset

    def filter_upper_bits(icolor:int, color_count:int):
        icolor = icolor % 128
        if color_count < 64:
            icolor = icolor % 64
        if color_count < 32:    
            icolor = icolor % 32
        if color_count < 16:
            icolor = icolor % 16
        return icolor

    def read_image(begin : int, length: int):
        out_image = np.array([], dtype=np.int8)
        total_len = 0
        i_ = begin
        while i_ < begin + length:
            current_color = data[i_]

            if (current_color >= 128):
                if color_array_len <= 16:
                    inline_rle = int("0b0"+bin(current_color)[3:6], 2)
                else:
                    inline_rle = 0

                current_color = filter_upper_bits(current_color, color_array_len)
                
                if inline_rle == 0:
                    i_ += 1
                    color_length = data[i_]

                    if color_length > 127:
                        color_length = color_length - 1

                    #print(f'rle compressed color {current_color} with length {color_length}')
                    total_len += color_length
                    out_image = np.append(out_image, np.tile(np.array([current_color]), color_length))
                else:
                    #print(f'rle inline compressed color {current_color} with length {inline_rle}')
                    out_image = np.append(out_image, np.tile(np.array([current_color]), inline_rle))
                    total_len += inline_rle
                
            else:
                total_len += 1
                #print(f'normal color {current_color} len 1')
                out_image = np.append(out_image, np.array([current_color]))

            i_ += 1

        pixel_loss = (image_x * image_y) - total_len

        #print(f"got length {total_len} when {int(image_x) * int(image_y)} is intended (probably)")
        #print(f"x {int(image_x)} y {int(image_y)} color count {color_array_len}")

        if pixel_loss > 0:
            print(f"whered you lose {pixel_loss} pixels huh")
            out_image = np.concatenate([out_image, np.zeros((pixel_loss))], axis=0)

        if pixel_loss < 0:
            print(f"whered you GAIN {pixel_loss * -1} pixels huh")
            out_image = out_image[:(image_x * image_y)]

        output_images.append(out_image)

    encoded_images = []

    for i in range(images_stored):
        chunk_len = get_chunk_length(current_read_offset)
        #print(f"chunk should be {chunk_len} in length")
        encoded_images.append(EncodedImage(current_read_offset+4, chunk_len))
        current_read_offset += chunk_len + 4

    #init_offset + 1
    enc_i : EncodedImage
    for enc_i in encoded_images:
        read_image(enc_i.offset, enc_i.length)

    output_images_colored = []

    dict_vec_thing = np.vectorize(color_table.__getitem__)

    for i in output_images:
        colored_img = []
        for j in i:
            max_color = max(color_table.keys())
            j = j % (max_color+1)
            colored_img.append(color_table[j])
        colored_img = np.array(colored_img)
        output_images_colored.append(colored_img)

    repeat = True
    if images_stored == 1:
        repeat = False

    # fig, ax = plt.subplots()

    # ims = []
    # for i, limg in enumerate(output_images_colored):
    #     limg = limg.reshape(image_y, image_x, 4)
    #     im = ax.imshow(limg, animated=True, interpolation="nearest")
    #     if i == 0:
    #         ax.imshow(limg, interpolation="nearest")  # show an initial one first
    #     ims.append([im])

    # ani = animation.ArtistAnimation(fig, ims, interval=100, blit=True, repeat_delay=0, repeat = repeat)
    #plt.show()

    if len(output_images_colored) == 1:
        Image.fromarray(output_images_colored[0].reshape(image_y, image_x, 4), 'RGBA').save(f'{path_to_save_folder}\\{img_name[:-4]}.png')
    else:
        for i, img_ in enumerate(output_images_colored):
            Image.fromarray(img_.reshape(image_y, image_x, 4), 'RGBA').save(f'{path_to_save_folder}\\{img_name[:-4]}_{i}.png')

files_thing = [f for f in listdir(path_to_folder) if isfile(join(path_to_folder, f))]
spt_files = []
for i in range(len(files_thing)):
    if files_thing[i].lower().endswith('.spt'):
        spt_files.append(files_thing[i])

for i in spt_files:
    read_spt_file(f'{path_to_folder}\\{i}', i)
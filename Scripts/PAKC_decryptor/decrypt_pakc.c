#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t P_ARRAY[18];
static uint32_t S_BOXES[1024];

#define CONTEXT_BUFFER_SIZE 4044

const char *ARCHIVE_KEYS[5] = {
    "asefcsee",
    "sddfcer4",
    "3434frdc",
    "fvbtgrsf",
    "34fgrfgf"
};

const char *ARCHIVE_FILES[5] = {
    "6r45-zz01.pakc",
    "6r45-zz02.pakc",
    "6r45-zz03.pakc",
    "6r45-zz04.pakc",
    "6r45-zz05.pakc"
};

void print_hex_buffer(const char *label, const uint8_t *buffer, size_t size)
{
    printf("%s (%zu bytes):\n", label, size);
    for (size_t i = 0; i < size; i++)
    {
        printf("%02x ", buffer[i]);
        if ((i + 1) % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n\n");
}

int load_binary_files()
{
    FILE *fp;

    fp = fopen("P_ARRAY_7F4D0.bin", "rb");
    if (!fp)
    {
        perror("cant open P_ARRAY_7F4D0.bin");
        return 0;
    }
    if (fread(P_ARRAY, 1, 72, fp) != 72)
    {
        fclose(fp);
        perror("cant read P_ARRAY_7F4D0.bin");
        return 0;
    }

    // print_hex_buffer("unk_7F4D0 contents", (uint8_t *)unk_7F4D0, 72);

    fclose(fp);

    fp = fopen("S_BOXES_7F518.bin", "rb");
    if (!fp)
    {
        perror("cant open S_BOXES_7F518.bin");
        return 0;
    }
    if (fread(S_BOXES, 1, 4096, fp) != 4096)
    {
        fclose(fp);
        perror("cant read S_BOXES_7F518.bin");
        return 0;
    }
    fclose(fp);

    return 1;
}

#define HIBYTE(x) (((x) >> 24) & 0xFF)
#define BYTE2(x) (((x) >> 16) & 0xFF)
#define BYTE1(x) (((x) >> 8) & 0xFF)

typedef uint32_t _DWORD;
typedef uint16_t _WORD;
typedef uint8_t _BYTE;

int decryptPAKC_step6_7754C(int a1, unsigned int a2)
{
    return ((*(uint32_t *)(a1 + 4 * HIBYTE(a2) + 0x58) + *(uint32_t *)(a1 + ((a2 & 0xFF0000) >> 0xE) + 0x458)) ^ *(uint32_t *)(a1 + ((uint16_t)(a2 & 0xFF00) >> 6) + 0x858)) + *(uint32_t *)(a1 + 4 * (uint8_t)a2 + 0xC58);
} // IDK what this is I just copied it from ida and it works

int decryptPAKC_step7_702F0(uint32_t *key_buffer, int *output)
{
    int v4 = output[1];
    int v5 = key_buffer[4] ^ output[0];

    // 16 rounds of encryption
    int v6 = decryptPAKC_step6_7754C((int)key_buffer, v5) ^ v4 ^ key_buffer[5];
    int v7 = decryptPAKC_step6_7754C((int)key_buffer, v6) ^ v5 ^ key_buffer[6];
    int v8 = decryptPAKC_step6_7754C((int)key_buffer, v7) ^ v6 ^ key_buffer[7];
    int v9 = decryptPAKC_step6_7754C((int)key_buffer, v8) ^ v7 ^ key_buffer[8];
    int v10 = decryptPAKC_step6_7754C((int)key_buffer, v9) ^ v8 ^ key_buffer[9];
    int v11 = decryptPAKC_step6_7754C((int)key_buffer, v10) ^ v9 ^ key_buffer[10];
    int v12 = decryptPAKC_step6_7754C((int)key_buffer, v11) ^ v10 ^ key_buffer[11];
    int v13 = decryptPAKC_step6_7754C((int)key_buffer, v12) ^ v11 ^ key_buffer[12];
    int v14 = decryptPAKC_step6_7754C((int)key_buffer, v13) ^ v12 ^ key_buffer[13];
    int v15 = decryptPAKC_step6_7754C((int)key_buffer, v14) ^ v13 ^ key_buffer[14];
    int v16 = decryptPAKC_step6_7754C((int)key_buffer, v15) ^ v14 ^ key_buffer[15];
    int v17 = decryptPAKC_step6_7754C((int)key_buffer, v16) ^ v15 ^ key_buffer[16];
    int v18 = decryptPAKC_step6_7754C((int)key_buffer, v17) ^ v16 ^ key_buffer[17];
    int v19 = decryptPAKC_step6_7754C((int)key_buffer, v18) ^ v17 ^ key_buffer[18];
    int v20 = decryptPAKC_step6_7754C((int)key_buffer, v19) ^ v18 ^ key_buffer[19];
    int result = decryptPAKC_step6_7754C((int)key_buffer, v20) ^ v19 ^ key_buffer[20];

    // update output values
    output[1] = result;
    output[0] = key_buffer[21] ^ v20;

    return result;
}

uint32_t *decryptPAKC_step1_7049C(
    uint32_t *key_buffer,
    const void *input_data,
    unsigned int input_len,
    uint32_t *init_values)
{
    unsigned int adjusted_len = input_len;
    uint8_t temp_buffer[76];
    int step7_output[2];

    // IV is usually all 0 i guess. if that's what it's called
    key_buffer[0] = init_values[0];
    key_buffer[1] = init_values[1];
    key_buffer[2] = init_values[0];
    key_buffer[3] = init_values[1];

    // very usual blowfish key len limit to 56 bytes
    if (adjusted_len > 0x38)
    {
        adjusted_len = 0x38;
    }

    // the keys are usually 8 bytes long here though and thats what it copies
    memcpy(temp_buffer, input_data, adjusted_len);

    // copy the sboxes and parray. they are very usual blowfish ones, but in reverse endianness, each long is reversed
    memcpy(key_buffer + 4, P_ARRAY, 72);
    memcpy(key_buffer + 22, S_BOXES, 4096);

    const uint8_t *data_ptr = temp_buffer;
    int data_pos = 0;

    for (unsigned int i = 0; i < 18; ++i)
    {
        uint32_t value = 0;

        for (int j = 3; j >= 0; j--) // until (j-- != 0)
        {
            uint8_t byte = *data_ptr++;
            data_pos++;

            if (data_pos == adjusted_len)
            {
                data_pos = 0;
                data_ptr = temp_buffer;
            }

            // value from these bytes (4?)
            value = (value << 8) | byte;
        }

        // XOR with key material
        key_buffer[i + 4] ^= value; // v14 = &key_buffer_idk[i]; v14[4] ^= v9;
    }

    // init these (v24;v25)
    step7_output[0] = 0;
    step7_output[1] = 0;

    // first 18 copied dwords
    for (unsigned int j = 0; j < 18; j += 2)
    {
        decryptPAKC_step7_702F0(key_buffer, step7_output);
        key_buffer[j + 4] = step7_output[0];
        key_buffer[j + 5] = step7_output[1];
    }

    // process last 1024 things in blocks of 256 ig
    for (unsigned int k = 0; k < 4; ++k)
    {
        uint32_t *block_ptr = key_buffer + 256 * k;

        for (unsigned int m = 0; m < 256; m += 2)
        {
            decryptPAKC_step7_702F0(key_buffer, step7_output);
            block_ptr[m + 22] = step7_output[0];
            block_ptr[m + 23] = step7_output[1];
        }
    }

    return key_buffer;
}

int decryptPAKC_step3_76198(uint8_t *input, int32_t *output) // 8 bytes to 2 dwords
{
    output[0] = (input[0] << 24) |
                (input[1] << 16) |
                (input[2] << 8) |
                input[3];

    output[1] = (input[4] << 24) |
                (input[5] << 16) |
                (input[6] << 8) |
                input[7];

    return output[1];
}

int decryptPAKC_step5_7613C(int32_t *block, int output_offset)
{
    // writes data in reverse endianness? kind of like step3? idk

    uint8_t *output_ptr = (uint8_t *)output_offset;

    output_ptr[3] = (block[0] >> 0) & 0xFF;
    output_ptr[2] = (block[0] >> 8) & 0xFF;
    output_ptr[1] = (block[0] >> 16) & 0xFF;
    output_ptr[0] = (block[0] >> 24) & 0xFF;

    output_ptr[7] = (block[1] >> 0) & 0xFF;
    output_ptr[6] = (block[1] >> 8) & 0xFF;
    output_ptr[5] = (block[1] >> 16) & 0xFF;
    output_ptr[4] = (block[1] >> 24) & 0xFF;

    return output_offset + 8;
}

void decryptPAKC_step4_70758(uint32_t *key_buffer, int32_t *output)
{
    int v4 = output[1];
    int v5 = key_buffer[21] ^ output[0];

    // Chain of 16 transformation rounds
    int v6 = decryptPAKC_step6_7754C((int)key_buffer, v5) ^ v4 ^ key_buffer[20];
    int v7 = decryptPAKC_step6_7754C((int)key_buffer, v6) ^ v5 ^ key_buffer[19];
    int v8 = decryptPAKC_step6_7754C((int)key_buffer, v7) ^ v6 ^ key_buffer[18];
    int v9 = decryptPAKC_step6_7754C((int)key_buffer, v8) ^ v7 ^ key_buffer[17];
    int v10 = decryptPAKC_step6_7754C((int)key_buffer, v9) ^ v8 ^ key_buffer[16];
    int v11 = decryptPAKC_step6_7754C((int)key_buffer, v10) ^ v9 ^ key_buffer[15];
    int v12 = decryptPAKC_step6_7754C((int)key_buffer, v11) ^ v10 ^ key_buffer[14];
    int v13 = decryptPAKC_step6_7754C((int)key_buffer, v12) ^ v11 ^ key_buffer[13];
    int v14 = decryptPAKC_step6_7754C((int)key_buffer, v13) ^ v12 ^ key_buffer[12];
    int v15 = decryptPAKC_step6_7754C((int)key_buffer, v14) ^ v13 ^ key_buffer[11];
    int v16 = decryptPAKC_step6_7754C((int)key_buffer, v15) ^ v14 ^ key_buffer[10];
    int v17 = decryptPAKC_step6_7754C((int)key_buffer, v16) ^ v15 ^ key_buffer[9];
    int v18 = decryptPAKC_step6_7754C((int)key_buffer, v17) ^ v16 ^ key_buffer[8];
    int v19 = decryptPAKC_step6_7754C((int)key_buffer, v18) ^ v17 ^ key_buffer[7];
    int v20 = decryptPAKC_step6_7754C((int)key_buffer, v19) ^ v18 ^ key_buffer[6];

    int result = decryptPAKC_step6_7754C((int)key_buffer, v20) ^ v19 ^ key_buffer[5];

    output[1] = result;
    output[0] = key_buffer[4] ^ v20;
}

int decryptPAKC_step2_70BF0(uint32_t *ctx_buf, uint8_t *input_data, int output_offset, unsigned int data_size, int mode)
{
    if (mode != 0)
    {
        perror("Literally mode 0 is the only hardcoded one Just give up Like what are you doing");
        return 0;
    }

    unsigned int remaining = data_size;
    int block[2] = {0};

    while (remaining >= 8)
    {
        decryptPAKC_step3_76198(input_data, block);

        decryptPAKC_step4_70758(ctx_buf, block);

        output_offset = decryptPAKC_step5_7613C(block, output_offset);

        remaining -= 8;
        input_data += 8;
    }

    return output_offset;
}

void print_usage(const char *program_name)
{
    printf("Usage: %s <archive_number>\n", program_name);
    printf("  archive_number: which of the 5 PAKC files for snakes 0.6.0.19 A3 to decrypt\n");
    printf("  you need both dword_7F518.bin and unk_7F4D0.bin in the same folder as the exe I think?\n");
    printf("  also each *.pakc should be called 6r45-zz0<number here>.pakc\n");
}

void dispose_first_word(uint8_t *data, size_t *size)
{
    memmove(data, data + sizeof(uint32_t), *size - sizeof(uint32_t));
    *size -= sizeof(uint32_t);
}

int main(int argc, char *argv[])
{
    int archive_number = 0;

    if (argc != 2)
    {
        print_usage(argv[0]);
    }
    else
    {
        archive_number = atoi(argv[1]);
    }

    if (archive_number < 0 || archive_number > 4)
    {
        fprintf(stderr, "theres only 5 PAKCs\n");
        print_usage(argv[0]);
        return 1;
    }

    uint32_t context_buffer[CONTEXT_BUFFER_SIZE / sizeof(uint32_t)] = {0};

    uint32_t init_values[2] = {0, 0};

    load_binary_files();

    decryptPAKC_step1_7049C(context_buffer, ARCHIVE_KEYS[archive_number], 8, init_values);

    printf("Step 1 should be done by now...\n");
    // print_hex_buffer("Final buffer thing", (uint8_t *)context_buffer, 32);

    // ok now step 2 (oo)

    // load pakc (only first one for now lol)

    FILE *fp = fopen(ARCHIVE_FILES[archive_number], "rb");
    if (!fp)
    {
        perror("cant open the pakc\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t *encrypted_data = malloc(file_size);
    if (!encrypted_data)
    {
        fclose(fp);
        fprintf(stderr, "malloc no enc data\n");
        return 1;
    }

    fread(encrypted_data, 1, file_size, fp);
    fclose(fp);

    // Prepare output buffer
    uint8_t *output_data = malloc(file_size);
    if (!output_data)
    {
        free(encrypted_data);
        fprintf(stderr, "malloc no out\n");
        return 1;
    }

    // step 2 (ooo)

    decryptPAKC_step2_70BF0(context_buffer, encrypted_data, (int)output_data, file_size, 0);

    // first word always seems to be garbage? not sure if this is a hack but it works i guess
    dispose_first_word(output_data, &file_size);

    print_hex_buffer("first 4 bytes of decrypted PAK", output_data, 4);

    if (output_data[0] != 0xF7 || output_data[1] != 0xD3 ||
        output_data[2] != 0x1F || output_data[3] != 0x10)
    {
        printf("...which dont match F7 D3 1F 10\n");
        printf("Found: %02X %02X %02X %02X\n",
               output_data[0], output_data[1],
               output_data[2], output_data[3]);
        printf("ahhhh something went WRONG");
    }
    else
    {
        printf("...which do match F7 D3 1F 10!! well done. use offzip on it\n");
    }

    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "6r45-zz0%d_decrypted.PAK", (archive_number + 1));

    FILE *out_fp = fopen(output_filename, "wb");
    if (!out_fp)
    {
        perror("cant make output file");
    }
    else
    {
        fwrite(output_data, 1, file_size, out_fp);
        fclose(out_fp);
        printf("\nsaved decrypted PAK to %s\n", output_filename);
    }

    free(encrypted_data);
    free(output_data);

    return 0;
}
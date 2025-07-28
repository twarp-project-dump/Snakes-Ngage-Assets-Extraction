#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t P_ARRAY[18];
static uint32_t S_BOXES[1024];

#define MODE_DECRYPT 0
#define MODE_ENCRYPT 1

#define CONTEXT_BUFFER_SIZE (22*4 + 1024*4)

const char *ARCHIVE_KEYS[5] = {
    "asefcsee",
    "sddfcer4",
    "3434frdc",
    "fvbtgrsf",
    "34fgrfgf"};

const char *ARCHIVE_FILES[5] = {
    "6r45-zz01.pakc",
    "6r45-zz02.pakc",
    "6r45-zz03.pakc",
    "6r45-zz04.pakc",
    "6r45-zz05.pakc"};

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

//had to rewrite this a lot compared to the original decomp so that gcc would stop complaining. I think it works
uint32_t decryptPAKC_step6_7754C(uint32_t *key_buffer, uint32_t a2) {
    uint8_t* base = (uint8_t*)key_buffer;
    uint32_t part1 = *(uint32_t*)(base + 0x58 + 4 * HIBYTE(a2));
    uint32_t part2 = *(uint32_t*)(base + 0x458 + ((a2 & 0xFF0000) >> 14));
    uint32_t part3 = *(uint32_t*)(base + 0x858 + ((a2 & 0xFF00) >> 6));
    uint32_t part4 = *(uint32_t*)(base + 0xC58 + 4 * (uint8_t)a2);
    
    return ((part1 + part2) ^ part3) + part4;
}


void decryptPAKC_step7_702F0(uint32_t *key_buffer, uint32_t *output)
{
    uint32_t v4 = output[1];
    uint32_t v5 = key_buffer[4] ^ output[0];

    uint32_t v6 = decryptPAKC_step6_7754C(key_buffer, v5) ^ v4 ^ key_buffer[5];
    uint32_t v7 = decryptPAKC_step6_7754C(key_buffer, v6) ^ v5 ^ key_buffer[6];
    uint32_t v8 = decryptPAKC_step6_7754C(key_buffer, v7) ^ v6 ^ key_buffer[7];
    uint32_t v9 = decryptPAKC_step6_7754C(key_buffer, v8) ^ v7 ^ key_buffer[8];
    uint32_t v10 = decryptPAKC_step6_7754C(key_buffer, v9) ^ v8 ^ key_buffer[9];
    uint32_t v11 = decryptPAKC_step6_7754C(key_buffer, v10) ^ v9 ^ key_buffer[10];
    uint32_t v12 = decryptPAKC_step6_7754C(key_buffer, v11) ^ v10 ^ key_buffer[11];
    uint32_t v13 = decryptPAKC_step6_7754C(key_buffer, v12) ^ v11 ^ key_buffer[12];
    uint32_t v14 = decryptPAKC_step6_7754C(key_buffer, v13) ^ v12 ^ key_buffer[13];
    uint32_t v15 = decryptPAKC_step6_7754C(key_buffer, v14) ^ v13 ^ key_buffer[14];
    uint32_t v16 = decryptPAKC_step6_7754C(key_buffer, v15) ^ v14 ^ key_buffer[15];
    uint32_t v17 = decryptPAKC_step6_7754C(key_buffer, v16) ^ v15 ^ key_buffer[16];
    uint32_t v18 = decryptPAKC_step6_7754C(key_buffer, v17) ^ v16 ^ key_buffer[17];
    uint32_t v19 = decryptPAKC_step6_7754C(key_buffer, v18) ^ v17 ^ key_buffer[18];
    uint32_t v20 = decryptPAKC_step6_7754C(key_buffer, v19) ^ v18 ^ key_buffer[19];
    uint32_t result = decryptPAKC_step6_7754C(key_buffer, v20) ^ v19 ^ key_buffer[20];

    output[1] = result;
    output[0] = key_buffer[21] ^ v20;
}

uint32_t *decryptPAKC_step1_7049C(
    uint32_t *key_buffer,
    const void *input_data,
    unsigned int input_len,
    uint32_t *init_values)
{
    unsigned int adjusted_len = input_len;
    uint8_t temp_buffer[76];
    uint32_t step7_output[2];

    // this is the IV i guess but it should always be 0 (?)
    key_buffer[0] = init_values[0];
    key_buffer[1] = init_values[1];
    key_buffer[2] = init_values[0];
    key_buffer[3] = init_values[1];

    // standard 56 bytes key len limit
    if (adjusted_len > 0x38) {
        adjusted_len = 0x38;
    }

    memcpy(temp_buffer, input_data, adjusted_len);

    memcpy(key_buffer + 4, P_ARRAY, 72);
    memcpy(key_buffer + 22, S_BOXES, 4096);

    const uint8_t *data_ptr = temp_buffer;
    int data_pos = 0;

    for (unsigned int i = 0; i < 18; ++i) {
        uint32_t value = 0;

        for (int j = 3; j >= 0; j--) {
            uint8_t byte = *data_ptr++;
            data_pos++;

            if (data_pos == adjusted_len) {
                data_pos = 0;
                data_ptr = temp_buffer;
            }

            value = (value << 8) | byte;
        }

        key_buffer[i + 4] ^= value;
    }

    step7_output[0] = 0;
    step7_output[1] = 0;

    for (unsigned int j = 0; j < 18; j += 2) {
        decryptPAKC_step7_702F0(key_buffer, step7_output);
        key_buffer[j + 4] = step7_output[0];
        key_buffer[j + 5] = step7_output[1];
    }

    for (unsigned int k = 0; k < 4; ++k) {
        uint32_t *block_ptr = (uint32_t*)((uint8_t*)key_buffer + 88 + (k * 1024));

        for (unsigned int m = 0; m < 256; m += 2) {
            decryptPAKC_step7_702F0(key_buffer, step7_output);
            block_ptr[m] = step7_output[0];
            block_ptr[m + 1] = step7_output[1];
        }
    }

    return key_buffer;
}

void decryptPAKC_step3_76198(uint8_t *input, uint32_t *output)
{
    output[0] = (input[0] << 24) |
                (input[1] << 16) |
                (input[2] << 8) |
                input[3];

    output[1] = (input[4] << 24) |
                (input[5] << 16) |
                (input[6] << 8) |
                input[7];
}

void decryptPAKC_step5_7613C(uint32_t *block, uint8_t *output_ptr)
{
    output_ptr[3] = (block[0] >> 0) & 0xFF;
    output_ptr[2] = (block[0] >> 8) & 0xFF;
    output_ptr[1] = (block[0] >> 16) & 0xFF;
    output_ptr[0] = (block[0] >> 24) & 0xFF;

    output_ptr[7] = (block[1] >> 0) & 0xFF;
    output_ptr[6] = (block[1] >> 8) & 0xFF;
    output_ptr[5] = (block[1] >> 16) & 0xFF;
    output_ptr[4] = (block[1] >> 24) & 0xFF;
}

void decryptPAKC_step4_70758(uint32_t *key_buffer, uint32_t *output)
{
    uint32_t v4 = output[1];
    uint32_t v5 = key_buffer[21] ^ output[0];

    uint32_t v6 = decryptPAKC_step6_7754C(key_buffer, v5) ^ v4 ^ key_buffer[20];
    uint32_t v7 = decryptPAKC_step6_7754C(key_buffer, v6) ^ v5 ^ key_buffer[19];
    uint32_t v8 = decryptPAKC_step6_7754C(key_buffer, v7) ^ v6 ^ key_buffer[18];
    uint32_t v9 = decryptPAKC_step6_7754C(key_buffer, v8) ^ v7 ^ key_buffer[17];
    uint32_t v10 = decryptPAKC_step6_7754C(key_buffer, v9) ^ v8 ^ key_buffer[16];
    uint32_t v11 = decryptPAKC_step6_7754C(key_buffer, v10) ^ v9 ^ key_buffer[15];
    uint32_t v12 = decryptPAKC_step6_7754C(key_buffer, v11) ^ v10 ^ key_buffer[14];
    uint32_t v13 = decryptPAKC_step6_7754C(key_buffer, v12) ^ v11 ^ key_buffer[13];
    uint32_t v14 = decryptPAKC_step6_7754C(key_buffer, v13) ^ v12 ^ key_buffer[12];
    uint32_t v15 = decryptPAKC_step6_7754C(key_buffer, v14) ^ v13 ^ key_buffer[11];
    uint32_t v16 = decryptPAKC_step6_7754C(key_buffer, v15) ^ v14 ^ key_buffer[10];
    uint32_t v17 = decryptPAKC_step6_7754C(key_buffer, v16) ^ v15 ^ key_buffer[9];
    uint32_t v18 = decryptPAKC_step6_7754C(key_buffer, v17) ^ v16 ^ key_buffer[8];
    uint32_t v19 = decryptPAKC_step6_7754C(key_buffer, v18) ^ v17 ^ key_buffer[7];
    uint32_t v20 = decryptPAKC_step6_7754C(key_buffer, v19) ^ v18 ^ key_buffer[6];
    uint32_t result = decryptPAKC_step6_7754C(key_buffer, v20) ^ v19 ^ key_buffer[5];

    output[1] = result;
    output[0] = key_buffer[4] ^ v20;
}

void encryptPAKC_step4_inverse(uint32_t *key_buffer, uint32_t *output)
{
    uint32_t v4 = output[1];
    uint32_t v5 = key_buffer[4] ^ output[0];

    uint32_t v6 = decryptPAKC_step6_7754C(key_buffer, v5) ^ v4 ^ key_buffer[5];
    uint32_t v7 = decryptPAKC_step6_7754C(key_buffer, v6) ^ v5 ^ key_buffer[6];
    uint32_t v8 = decryptPAKC_step6_7754C(key_buffer, v7) ^ v6 ^ key_buffer[7];
    uint32_t v9 = decryptPAKC_step6_7754C(key_buffer, v8) ^ v7 ^ key_buffer[8];
    uint32_t v10 = decryptPAKC_step6_7754C(key_buffer, v9) ^ v8 ^ key_buffer[9];
    uint32_t v11 = decryptPAKC_step6_7754C(key_buffer, v10) ^ v9 ^ key_buffer[10];
    uint32_t v12 = decryptPAKC_step6_7754C(key_buffer, v11) ^ v10 ^ key_buffer[11];
    uint32_t v13 = decryptPAKC_step6_7754C(key_buffer, v12) ^ v11 ^ key_buffer[12];
    uint32_t v14 = decryptPAKC_step6_7754C(key_buffer, v13) ^ v12 ^ key_buffer[13];
    uint32_t v15 = decryptPAKC_step6_7754C(key_buffer, v14) ^ v13 ^ key_buffer[14];
    uint32_t v16 = decryptPAKC_step6_7754C(key_buffer, v15) ^ v14 ^ key_buffer[15];
    uint32_t v17 = decryptPAKC_step6_7754C(key_buffer, v16) ^ v15 ^ key_buffer[16];
    uint32_t v18 = decryptPAKC_step6_7754C(key_buffer, v17) ^ v16 ^ key_buffer[17];
    uint32_t v19 = decryptPAKC_step6_7754C(key_buffer, v18) ^ v17 ^ key_buffer[18];
    uint32_t v20 = decryptPAKC_step6_7754C(key_buffer, v19) ^ v18 ^ key_buffer[19];
    uint32_t result = decryptPAKC_step6_7754C(key_buffer, v20) ^ v19 ^ key_buffer[20];

    output[1] = result;
    output[0] = key_buffer[21] ^ v20;
}

void decryptPAKC_step2_70BF0(uint32_t *ctx_buf, uint8_t *input_data, uint8_t *output_data, unsigned int data_size, int mode)
{
    unsigned int remaining = data_size;
    uint32_t block[2] = {0};

    while (remaining >= 8)
    {
        decryptPAKC_step3_76198(input_data, block);

        if (mode == MODE_DECRYPT)
        {
            decryptPAKC_step4_70758(ctx_buf, block);
        }
        else
        {
            encryptPAKC_step4_inverse(ctx_buf, block);
        }

        decryptPAKC_step5_7613C(block, output_data);

        remaining -= 8;
        input_data += 8;
        output_data += 8;
    }
}

void print_usage(const char *program_name)
{
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -i <file>    Input file to process\n");
    printf("  -o <file>    Output file\n");
    printf("  -k <key>     Encryption/decryption key (string)\n");
    printf("  -kf <file>   Read key from file\n");
    printf("  -n <num>     Use predefined key number (1-5)\n");
    printf("  -d           Decrypt mode (default)\n");
    printf("  -e           Encrypt mode\n");
    printf("  -h           Show this help message\n");
    printf("\nPredefined keys:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("  %d: %s (for %s)\n", i + 1, ARCHIVE_KEYS[i], ARCHIVE_FILES[i]);
    }
    printf("\nExamples:\n");
    printf("  %s -i encrypted.pakc -o decrypted.pak -k \"mysecretkey\" -d\n", program_name);
    printf("  %s -i 6r45-zz01.pakc -o decrypted.pak -n 1 -d\n", program_name);
    printf("  %s -i decrypted.pak -o encrypted.pakc -n 2 -e\n", program_name);
}

int verify_decrypted_header(const uint8_t *data)
{
    return (data[4] == 0xF7 && data[5] == 0xD3 &&
            data[6] == 0x1F && data[7] == 0x10);
}

int main(int argc, char *argv[])
{
    char *input_filename = NULL;
    char *output_filename = NULL;
    char *key = NULL;
    char *key_file = NULL;
    int mode = MODE_DECRYPT;
    int predefined_key = -1;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
        {
            input_filename = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
        {
            output_filename = argv[++i];
        }
        else if (strcmp(argv[i], "-k") == 0 && i + 1 < argc)
        {
            key = argv[++i];
        }
        //really why is this here at all idk
        else if (strcmp(argv[i], "-kf") == 0 && i + 1 < argc)
        {
            key_file = argv[++i];
        }
        else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc)
        {
            predefined_key = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            mode = MODE_DECRYPT;
        }
        else if (strcmp(argv[i], "-e") == 0)
        {
            mode = MODE_ENCRYPT;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            print_usage(argv[0]);
            return 0;
        }
        else
        {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (!input_filename)
    {
        fprintf(stderr, "Error: input file must be specified with -i\n");
        print_usage(argv[0]);
        return 1;
    }

    // Handle key selection
    if (predefined_key != -1)
    {
        if (predefined_key < 1 || predefined_key > 5)
        {
            fprintf(stderr, "Error: predefined key number must be between 1 and 5\n");
            print_usage(argv[0]);
            return 1;
        }
        key = (char *)ARCHIVE_KEYS[predefined_key - 1];
    }
    else if (!key && !key_file)
    {
        fprintf(stderr, "Error: no key specified! Use -k, -kf or -n\n");
        print_usage(argv[0]);
        return 1;
    }

    if (!output_filename)
    {
        // Generate default output filename based on mode
        size_t len = strlen(input_filename) + 16;
        output_filename = (char *)malloc(len);
        if (mode == MODE_DECRYPT)
        {
            snprintf(output_filename, len, "%s.decrypted", input_filename);
        }
        else
        {
            snprintf(output_filename, len, "%s.encrypted", input_filename);
        }
    }

    if (key_file)
    {
        // Read key from file
        FILE *kf = fopen(key_file, "rb");
        if (!kf)
        {
            perror("cant open key file");
            return 1;
        }
        fseek(kf, 0, SEEK_END);
        long key_size = ftell(kf);
        fseek(kf, 0, SEEK_SET);

        key = (char *)malloc(key_size + 1);
        if (!key)
        {
            fclose(kf);
            fprintf(stderr, "cant allocate memory\n");
            return 1;
        }

        size_t bytes_read = fread(key, 1, key_size, kf);
        key[bytes_read] = '\0';
        fclose(kf);
    }

    printf("Processing with parameters:\n");
    printf("  Input file: %s\n", input_filename);
    printf("  Output file: %s\n", output_filename);
    printf("  Mode: %s\n", mode == MODE_DECRYPT ? "Decrypt" : "Encrypt");
    if (predefined_key != -1)
    {
        printf("  Using predefined key #%d: %s\n", predefined_key, key);
    }
    else
    {
        printf("  Key: %s\n", key_file ? "(from file)" : key);
    }

    uint32_t context_buffer[CONTEXT_BUFFER_SIZE / sizeof(uint32_t)] = {0};
    uint32_t init_values[2] = {0, 0};

    if (!load_binary_files())
    {
        fprintf(stderr, "couldnt load binary files\n");
        if (key_file && predefined_key == -1)
            free(key);
        if (!output_filename)
            free(output_filename);
        return 1;
    }

    decryptPAKC_step1_7049C(context_buffer, key, strlen(key), init_values);

    FILE *fp = fopen(input_filename, "rb");
    if (!fp)
    {
        perror("cant open input file");
        if (key_file && predefined_key == -1)
            free(key);
        if (!output_filename)
            free(output_filename);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t *input_data = (uint8_t *)malloc(file_size);
    if (!input_data)
    {
        fclose(fp);
        fprintf(stderr, "cant allocate memory\n");
        if (key_file && predefined_key == -1)
            free(key);
        if (!output_filename)
            free(output_filename);
        return 1;
    }

    fread(input_data, 1, file_size, fp);
    fclose(fp);

    uint8_t *output_data = (uint8_t *)malloc(file_size);
    if (!output_data)
    {
        free(input_data);
        fprintf(stderr, "cant allocate memory\n");
        if (key_file && predefined_key == -1)
            free(key);
        if (!output_filename)
            free(output_filename);
        return 1;
    }

    decryptPAKC_step2_70BF0(context_buffer, input_data, output_data, file_size, mode);

    if (mode == MODE_DECRYPT)
    {
        print_hex_buffer("First 8 bytes of decrypted data", output_data, 8);

        if (verify_decrypted_header(output_data))
        {
            printf("the header (F7 D3 1F 10) seems to be present. good!\n");
        }
        else
        {
            printf("no header present (F7 D3 1F 10) in the decrypted file. not good.\n");
        }
    }


    FILE *out_fp = fopen(output_filename, "wb");
    if (!out_fp)
    {
        perror("cant create output file");
        free(input_data);
        free(output_data);
        if (key_file && predefined_key == -1)
            free(key);
        if (!output_filename)
            free(output_filename);
        return 1;
    }

    fwrite(output_data, 1, file_size, out_fp);
    fclose(out_fp);

    printf("Successfully wrote output to %s\n", output_filename);

    free(input_data);
    free(output_data);
    if (key_file && predefined_key == -1)
        free(key);
    if (!output_filename)
        free(output_filename);

    return 0;
}
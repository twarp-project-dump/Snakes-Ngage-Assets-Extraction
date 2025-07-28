#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define HIBYTE(x) (((x) >> 24) & 0xFF)
#define BYTE2(x) (((x) >> 16) & 0xFF)
#define BYTE1(x) (((x) >> 8) & 0xFF)

typedef uint32_t _DWORD;
typedef uint16_t _WORD;
typedef uint8_t _BYTE;

// ROtate Right
#define __ROR4__(value, shift) (((value) >> (shift)) | ((value) << (32 - (shift))))

// MD5 hash thingy
uint32_t md5_hash_part_lol(uint32_t *result, const uint32_t *a2)
{
    uint32_t v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17;
    uint32_t v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33;
    uint32_t v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49;
    uint32_t v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63, v64, v65;
    uint32_t v66, v67, v68, v69, v70, v71, v72, v73, v74, v75, v76, v77, v78, v79, v80, v81;
    uint32_t v82, v83, v84, v85, v86, v87, v88, v89, v90, v91, v92, v93, v94, v95, v96, v97;
    uint32_t v98, v99, v100, v101, v102, v103, v104, v105, v106, v107, v108, v109, v110, v111;
    uint32_t v112, v113, v114, v115, v116, v117, v118, v119, v120, v121, v122, v123, v124, v125;
    uint32_t v126, v127, v128, v129, v130, v131, v132, v133;

    uint32_t v134; // [sp+0h] [bp-58h]
    uint32_t v135; // [sp+4h] [bp-54h]
    uint32_t v136; // [sp+8h] [bp-50h]
    uint32_t v137; // [sp+Ch] [bp-4Ch]
    uint32_t v138; // [sp+10h] [bp-48h]
    uint32_t v139; // [sp+14h] [bp-44h]
    uint32_t v140; // [sp+18h] [bp-40h]
    uint32_t v141; // [sp+1Ch] [bp-3Ch]
    uint32_t v142; // [sp+20h] [bp-38h]
    uint32_t v143; // [sp+24h] [bp-34h]
    uint32_t v144; // [sp+28h] [bp-30h]
    uint32_t v145; // [sp+2Ch] [bp-2Ch]

    v145 = result[1];
    v144 = result[2];
    v143 = result[3];
    v142 = *a2;

    v3 = __ROR4__(a2[0] + result[0] + ((v144 ^ v143) & v145 ^ v143) - 0x28955B88, 0x19);
    v2 = v145 + v3;

    v141 = a2[1];
    v5 = __ROR4__(v141 + v143 + ((v145 ^ v144) & (v145 + v3) ^ v144) - 0x173848AA, 0x14);
    v4 = v2 + v5;

    v140 = a2[2];
    v7 = __ROR4__(v140 + v144 + ((v2 ^ v145) & (v2 + v5) ^ v145) + 0x242070DB, 0xF);
    v6 = v4 + v7;

    v139 = a2[3];
    v9 = __ROR4__(v139 + v145 + ((v4 ^ v2) & (v4 + v7) ^ v2) - 0x3E423112, 0xA);
    v8 = v6 + v9;

    v138 = a2[4];
    v11 = __ROR4__(v138 + v2 + ((v6 ^ v4) & (v6 + v9) ^ v4) - 0xA83F051, 0x19);
    v10 = v8 + v11;

    v137 = a2[5];
    v13 = __ROR4__(v137 + v4 + ((v8 ^ v6) & (v8 + v11) ^ v6) + 0x4787C62A, 0x14);
    v12 = v10 + v13;

    v136 = a2[6];
    v15 = __ROR4__(v136 + v6 + ((v10 ^ v8) & (v10 + v13) ^ v8) - 0x57CFB9ED, 0xF);
    v14 = v12 + v15;

    v135 = a2[7];
    v17 = __ROR4__(v135 + v8 + ((v12 ^ v10) & (v12 + v15) ^ v10) - 0x2B96AFF, 0xA);
    v16 = v14 + v17;

    v19 = __ROR4__(a2[8] + v10 + ((v14 ^ v12) & (v14 + v17) ^ v12) + 0x698098D8, 0x19);
    v18 = v16 + v19;

    v134 = a2[8];
    v20 = a2[9];
    v22 = __ROR4__(v20 + v12 + ((v16 ^ v14) & (v16 + v19) ^ v14) - 0x74BB0851, 0x14);
    v21 = v18 + v22;

    v23 = a2[0xA];
    v25 = __ROR4__(v23 + v14 + ((v18 ^ v16) & (v18 + v22) ^ v16) - 0xA44F, 0xF);
    v24 = v21 + v25;

    v26 = a2[0xB];
    v28 = __ROR4__(v26 + v16 + ((v21 ^ v18) & (v21 + v25) ^ v18) - 0x76A32842, 0xA);
    v27 = v24 + v28;

    v29 = a2[0xC];
    v31 = __ROR4__(v29 + v18 + ((v24 ^ v21) & (v24 + v28) ^ v21) + 0x6B901122, 0x19);
    v30 = v27 + v31;

    v32 = a2[0xD];
    v34 = __ROR4__(v32 + v21 + ((v27 ^ v24) & (v27 + v31) ^ v24) - 0x2678E6D, 0x14);
    v33 = v30 + v34;

    v35 = a2[0xE];
    v37 = __ROR4__(v35 + v24 + ((v30 ^ v27) & (v30 + v34) ^ v27) - 0x5986BC72, 0xF);
    v36 = v33 + v37;

    v38 = a2[0xF];
    v40 = __ROR4__(v38 + v27 + ((v33 ^ v30) & (v33 + v37) ^ v30) + 0x49B40821, 0xA);
    v39 = v36 + v40;

    v42 = __ROR4__(v141 + v30 + (((v36 + v40) ^ v36) & v33 ^ v36) - 0x9E1DA9E, 0x1B);
    v41 = v39 + v42;

    v44 = __ROR4__(v136 + v33 + (((v39 + v42) ^ v39) & v36 ^ v39) - 0x3FBF4CC0, 0x17);
    v43 = v41 + v44;

    v46 = __ROR4__(v26 + v36 + (((v41 + v44) ^ v41) & v39 ^ v41) + 0x265E5A51, 0x12);
    v45 = v43 + v46;

    v48 = __ROR4__(v142 + v39 + (((v43 + v46) ^ v43) & v41 ^ v43) - 0x16493856, 0xC);
    v47 = v45 + v48;

    v50 = __ROR4__(v137 + v41 + (((v45 + v48) ^ v45) & v43 ^ v45) - 0x29D0EFA3, 0x1B);
    v49 = v47 + v50;

    v52 = __ROR4__(v23 + v43 + (((v47 + v50) ^ v47) & v45 ^ v47) + 0x2441453, 0x17);
    v51 = v49 + v52;

    v54 = __ROR4__(v38 + v45 + (((v49 + v52) ^ v49) & v47 ^ v49) - 0x275E197F, 0x12);
    v53 = v51 + v54;

    v56 = __ROR4__(v138 + v47 + (((v51 + v54) ^ v51) & v49 ^ v51) - 0x182C0438, 0xC);
    v55 = v53 + v56;

    v58 = __ROR4__(v20 + v49 + (((v53 + v56) ^ v53) & v51 ^ v53) + 0x21E1CDE6, 0x1B);
    v57 = v55 + v58;

    v60 = __ROR4__(v35 + v51 + (((v55 + v58) ^ v55) & v53 ^ v55) - 0x3CC8F82A, 0x17);
    v59 = v57 + v60;

    v62 = __ROR4__(v139 + v53 + (((v57 + v60) ^ v57) & v55 ^ v57) - 0xB2AF279, 0x12);
    v61 = v59 + v62;

    v64 = __ROR4__(v134 + v55 + (((v59 + v62) ^ v59) & v57 ^ v59) + 0x455A14ED, 0xC);
    v63 = v61 + v64;

    v66 = __ROR4__(v32 + v57 + (((v61 + v64) ^ v61) & v59 ^ v61) - 0x561C16FB, 0x1B);
    v65 = v63 + v66;

    v68 = __ROR4__(v140 + v59 + (((v63 + v66) ^ v63) & v61 ^ v63) - 0x3105C08, 0x17);
    v67 = v65 + v68;

    v70 = __ROR4__(v135 + v61 + (((v65 + v68) ^ v65) & v63 ^ v65) + 0x676F02D9, 0x12);
    v69 = v67 + v70;

    v72 = __ROR4__(v29 + v63 + (((v67 + v70) ^ v67) & v65 ^ v67) - 0x72D5B376, 0xC);
    v71 = v69 + v72;

    v74 = __ROR4__(v137 + v65 + ((v69 + v72) ^ v69 ^ v67) - 0x5C6BE, 0x1C);
    v73 = v71 + v74;

    v76 = __ROR4__(v134 + v67 + ((v71 + v74) ^ v71 ^ v69) - 0x788E097F, 0x15);
    v75 = v73 + v76;

    v78 = __ROR4__(v26 + v69 + ((v73 + v76) ^ v73 ^ v71) + 0x6D9D6122, 0x10);
    v77 = v75 + v78;

    v80 = __ROR4__(v35 + v71 + ((v75 + v78) ^ v75 ^ v73) - 0x21AC7F4, 9);
    v79 = v77 + v80;

    v82 = __ROR4__(v141 + v73 + ((v77 + v80) ^ v77 ^ v75) - 0x5B4115BC, 0x1C);
    v81 = v79 + v82;

    v84 = __ROR4__(v138 + v75 + ((v79 + v82) ^ v79 ^ v77) + 0x4BDECFA9, 0x15);
    v83 = v81 + v84;

    v86 = __ROR4__(v135 + v77 + ((v81 + v84) ^ v81 ^ v79) - 0x944B4A0, 0x10);
    v85 = v83 + v86;

    v88 = __ROR4__(v23 + v79 + ((v83 + v86) ^ v83 ^ v81) - 0x41404390, 9);
    v87 = v85 + v88;

    v90 = __ROR4__(v32 + v81 + ((v85 + v88) ^ v85 ^ v83) + 0x289B7EC6, 0x1C);
    v89 = v87 + v90;

    v92 = __ROR4__(v142 + v83 + ((v87 + v90) ^ v87 ^ v85) - 0x155ED806, 0x15);
    v91 = v89 + v92;

    v94 = __ROR4__(v139 + v85 + ((v89 + v92) ^ v89 ^ v87) - 0x2B10CF7B, 0x10);
    v93 = v91 + v94;

    v96 = __ROR4__(v136 + v87 + ((v91 + v94) ^ v91 ^ v89) + 0x4881D05, 9);
    v95 = v93 + v96;

    v98 = __ROR4__(v20 + v89 + ((v93 + v96) ^ v93 ^ v91) - 0x262B2FC7, 0x1C);
    v97 = v95 + v98;

    v100 = __ROR4__(v29 + v91 + ((v95 + v98) ^ v95 ^ v93) - 0x1924661B, 0x15);
    v99 = v97 + v100;

    v102 = __ROR4__(v38 + v93 + ((v97 + v100) ^ v97 ^ v95) + 0x1FA27CF8, 0x10);
    v101 = v99 + v102;

    v104 = __ROR4__(v140 + v95 + ((v99 + v102) ^ v99 ^ v97) - 0x3B53A99B, 9);
    v103 = v101 + v104;

    v106 = __ROR4__(v142 + v97 + (((v101 + v104) | ~v99) ^ v101) - 0xBD6DDBC, 0x1A);
    v105 = v103 + v106;

    v108 = __ROR4__(v135 + v99 + (((v103 + v106) | ~v101) ^ v103) + 0x432AFF97, 0x16);
    v107 = v105 + v108;

    v110 = __ROR4__(v35 + v101 + (((v105 + v108) | ~v103) ^ v105) - 0x546BDC59, 0x11);
    v109 = v107 + v110;

    v112 = __ROR4__(v137 + v103 + (((v107 + v110) | ~v105) ^ v107) - 0x36C5FC7, 0xB);
    v111 = v109 + v112;

    v114 = __ROR4__(v29 + v105 + (((v109 + v112) | ~v107) ^ v109) + 0x655B59C3, 0x1A);
    v113 = v111 + v114;

    v116 = __ROR4__(v139 + v107 + (((v111 + v114) | ~v109) ^ v111) - 0x70F3336E, 0x16);
    v115 = v113 + v116;

    v118 = __ROR4__(v23 + v109 + (((v113 + v116) | ~v111) ^ v113) - 0x100B83, 0x11);
    v117 = v115 + v118;

    v120 = __ROR4__(v141 + v111 + (((v115 + v118) | ~v113) ^ v115) - 0x7A7BA22F, 0xB);
    v119 = v117 + v120;

    v122 = __ROR4__(v134 + v113 + (((v117 + v120) | ~v115) ^ v117) + 0x6FA87E4F, 0x1A);
    v121 = v119 + v122;

    v124 = __ROR4__(v38 + v115 + (((v119 + v122) | ~v117) ^ v119) - 0x1D31920, 0x16);
    v123 = v121 + v124;

    v126 = __ROR4__(v136 + v117 + (((v121 + v124) | ~v119) ^ v121) - 0x5CFEBCEC, 0x11);
    v125 = v123 + v126;

    v128 = __ROR4__(v32 + v119 + (((v123 + v126) | ~v121) ^ v123) + 0x4E0811A1, 0xB);
    v127 = v125 + v128;

    v130 = __ROR4__(v138 + v121 + (((v125 + v128) | ~v123) ^ v125) - 0x8AC817E, 0x1A);
    v129 = v127 + v130;

    v132 = __ROR4__(v26 + v123 + (((v127 + v130) | ~v125) ^ v127) - 0x42C50DCB, 0x16);
    v131 = v129 + v132;

    v133 = __ROR4__(v140 + v125 + (((v129 + v132) | ~v127) ^ v129) + 0x2AD7D2BB, 0x11);

    result[0] += v129;
    result[1] = v145 + v131 + v133 + __ROR4__(v20 + v127 + (((v131 + v133) | ~v129) ^ v131) - 0x14792C6F, 0xB);
    result[2] = v144 + v131 + v133;
    result[3] = v143 + v131;
    return result;
}

void memclr(void *dest, size_t size)
{
    memset(dest, 0, size);
}

void update_md5_w_qmemcpy(_DWORD *a1, char *a2, size_t a3)
{
    size_t v3 = a3;
    _DWORD *v4 = a1;
    size_t v5 = a1[4];
    size_t v7 = v5 + 8 * a3;
    v4[4] = v7;

    bool v8 = v7 >= v5;
    if (!v8)
    {
        v4[5] = v7 + 1;
    }

    v4[5] += a3 >> 0x1D;
    size_t v9 = v5 << 0x17;
    char *v10 = (char *)(v9 >> 0x1A);

    if (v9 >> 0x1A)
    {
        size_t v11 = 64 - (size_t)v10;
        bool v12 = a3 >= 64 - (size_t)v10;
        char *v13 = &v10[(size_t)v4];

        if (!v12)
        {
            memcpy(v13 + 24, v10, a3);
            return;
        }

        memcpy(v13 + 24, a2, v11);
        md5_hash_part_lol(v4, v4 + 6);
        a2 += v11;
        v3 -= v11;
    }

    for (; v3 >= 64; a2 += 64)
    {
        memcpy(v4 + 6, a2, 64);
        md5_hash_part_lol(v4, v4 + 6);
        v3 -= 64;
    }

    memcpy(v4 + 6, a2, v3);
}

void finish_md5_hash(unsigned char *a1, _DWORD *a2)
{
    int v4 = (a2[4] << 0x17) >> 0x1A;
    char *v5 = (char *)a2 + v4;
    unsigned int v6 = 0x3F - v4;

    v5[0x18] = 0x80;
    char *v7 = v5 + 0x19;

    if (v6 >= 8)
    {
        memclr(v7, v6 - 8);
    }
    else
    {
        memclr(v7, v6);
        md5_hash_part_lol(a2, a2 + 6);
        memclr(a2 + 6, 0x38);
    }

    a2[0x14] = a2[4];
    a2[0x15] = a2[5];
    md5_hash_part_lol(a2, a2 + 6);

    if (a1 && a2)
    {
        for (int i = 0; i < 16; i++)
        {
            a1[i] = ((unsigned char *)a2)[i];
        }
    }

    memclr(a2, 4);
}

void encrypt_data(unsigned char *data, size_t data_size)
{
    uint32_t state1 = 0x89FD8833;
    uint32_t state2 = 0x2EB;
    uint32_t state3 = 0x13D4FD;

    for (size_t i = 0; i < data_size; i++)
    {
        uint32_t temp = state2;
        state2 = state1 * state2;
        state1 = temp;
        state3 = 0x78B7 * (state3 & 0xFFFF) + (state3 >> 16);
        data[i] ^= (state2 & 0xFF) + (state3 & 0xFF);
    }
}

int encode_sno_bin(const char *input_filename, const char *output_filename)
{
    FILE *file = fopen(input_filename, "rb");
    if (!file)
    {
        printf("Error opening file %s\n", input_filename);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size == 0)
    {
        printf("file empty\n");
        fclose(file);
        return -1;
    }

    unsigned char *file_data = (unsigned char *)malloc(file_size);
    if (!file_data)
    {
        printf("cant allocate memory\n");
        fclose(file);
        return -1;
    }

    fread(file_data, 1, file_size, file);
    fclose(file);

    int md5_state[22] = {0};
    md5_state[0] = 0x67452301;
    md5_state[1] = 0xEFCDAB89;
    md5_state[2] = 0x98BADCFE;
    md5_state[3] = 0x10325476;
    md5_state[4] = 0;
    md5_state[5] = 0;

    update_md5_w_qmemcpy(md5_state, (char *)file_data, file_size);

    unsigned char checksum[16];
    finish_md5_hash(checksum, md5_state);

    encrypt_data(file_data, file_size);

    size_t output_size = file_size + 16;
    unsigned char *output_data = (unsigned char *)malloc(output_size);
    if (!output_data)
    {
        printf("cant allocate memory\n");
        free(file_data);
        return -1;
    }

    memcpy(output_data, file_data, file_size);
    memcpy(output_data + file_size, checksum, 16);
    
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        printf("Error creating output file %s\n", output_filename);
        free(file_data);
        free(output_data);
        return -1;
    }

    fwrite(output_data, 1, output_size, output_file);
    fclose(output_file);

    printf("Successfully encoded file. Output saved to: %s\n", output_filename);
    printf("Original size: %ld bytes\n", file_size);
    printf("Output size: %zu bytes\n", output_size);
    printf("Checksum: ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02x", checksum[i]);
    }
    printf("\n");

    free(file_data);
    free(output_data);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <input.decrypted> <output.bin>\n", argv[0]);
        return 1;
    }

    return encode_sno_bin(argv[1], argv[2]);
}
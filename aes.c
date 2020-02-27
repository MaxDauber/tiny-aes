#include "aes.h"
#include "stdio.h"
#include <string.h>
#include <stdint.h>

/** Rijndael_S-box
 * copied from : https://en.wikipedia.org/wiki/Rijndael_S-box
 */
unsigned char sbox[256] =
        {
                0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
                0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
                0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
                0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
                0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
                0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
                0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
                0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
                0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
                0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
                0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
                0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
                0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
                0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
                0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
                0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
        };

/** Rijndael mix columns (LUT x2)
 * copied from : http://en.wikipedia.org/wiki/Rijndael_mix_columns
 *
 * 2x every byte
 */
unsigned char galois_2[256] =
        {
                0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e,
                0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e,
                0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e,
                0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e,
                0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e,
                0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe,
                0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 0xde,
                0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee, 0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe,
                0x1b, 0x19, 0x1f, 0x1d, 0x13, 0x11, 0x17, 0x15, 0x0b, 0x09, 0x0f, 0x0d, 0x03, 0x01, 0x07, 0x05,
                0x3b, 0x39, 0x3f, 0x3d, 0x33, 0x31, 0x37, 0x35, 0x2b, 0x29, 0x2f, 0x2d, 0x23, 0x21, 0x27, 0x25,
                0x5b, 0x59, 0x5f, 0x5d, 0x53, 0x51, 0x57, 0x55, 0x4b, 0x49, 0x4f, 0x4d, 0x43, 0x41, 0x47, 0x45,
                0x7b, 0x79, 0x7f, 0x7d, 0x73, 0x71, 0x77, 0x75, 0x6b, 0x69, 0x6f, 0x6d, 0x63, 0x61, 0x67, 0x65,
                0x9b, 0x99, 0x9f, 0x9d, 0x93, 0x91, 0x97, 0x95, 0x8b, 0x89, 0x8f, 0x8d, 0x83, 0x81, 0x87, 0x85,
                0xbb, 0xb9, 0xbf, 0xbd, 0xb3, 0xb1, 0xb7, 0xb5, 0xab, 0xa9, 0xaf, 0xad, 0xa3, 0xa1, 0xa7, 0xa5,
                0xdb, 0xd9, 0xdf, 0xdd, 0xd3, 0xd1, 0xd7, 0xd5, 0xcb, 0xc9, 0xcf, 0xcd, 0xc3, 0xc1, 0xc7, 0xc5,
                0xfb, 0xf9, 0xff, 0xfd, 0xf3, 0xf1, 0xf7, 0xf5, 0xeb, 0xe9, 0xef, 0xed, 0xe3, 0xe1, 0xe7, 0xe5
        };

/** Rijndael mix columns (LUT x3)
 * copied from : http://en.wikipedia.org/wiki/Rijndael_mix_columns
 *
 * 3x every byte
 */
unsigned char galois_3[256] =
        {
                0x00, 0x03, 0x06, 0x05, 0x0c, 0x0f, 0x0a, 0x09, 0x18, 0x1b, 0x1e, 0x1d, 0x14, 0x17, 0x12, 0x11,
                0x30, 0x33, 0x36, 0x35, 0x3c, 0x3f, 0x3a, 0x39, 0x28, 0x2b, 0x2e, 0x2d, 0x24, 0x27, 0x22, 0x21,
                0x60, 0x63, 0x66, 0x65, 0x6c, 0x6f, 0x6a, 0x69, 0x78, 0x7b, 0x7e, 0x7d, 0x74, 0x77, 0x72, 0x71,
                0x50, 0x53, 0x56, 0x55, 0x5c, 0x5f, 0x5a, 0x59, 0x48, 0x4b, 0x4e, 0x4d, 0x44, 0x47, 0x42, 0x41,
                0xc0, 0xc3, 0xc6, 0xc5, 0xcc, 0xcf, 0xca, 0xc9, 0xd8, 0xdb, 0xde, 0xdd, 0xd4, 0xd7, 0xd2, 0xd1,
                0xf0, 0xf3, 0xf6, 0xf5, 0xfc, 0xff, 0xfa, 0xf9, 0xe8, 0xeb, 0xee, 0xed, 0xe4, 0xe7, 0xe2, 0xe1,
                0xa0, 0xa3, 0xa6, 0xa5, 0xac, 0xaf, 0xaa, 0xa9, 0xb8, 0xbb, 0xbe, 0xbd, 0xb4, 0xb7, 0xb2, 0xb1,
                0x90, 0x93, 0x96, 0x95, 0x9c, 0x9f, 0x9a, 0x99, 0x88, 0x8b, 0x8e, 0x8d, 0x84, 0x87, 0x82, 0x81,
                0x9b, 0x98, 0x9d, 0x9e, 0x97, 0x94, 0x91, 0x92, 0x83, 0x80, 0x85, 0x86, 0x8f, 0x8c, 0x89, 0x8a,
                0xab, 0xa8, 0xad, 0xae, 0xa7, 0xa4, 0xa1, 0xa2, 0xb3, 0xb0, 0xb5, 0xb6, 0xbf, 0xbc, 0xb9, 0xba,
                0xfb, 0xf8, 0xfd, 0xfe, 0xf7, 0xf4, 0xf1, 0xf2, 0xe3, 0xe0, 0xe5, 0xe6, 0xef, 0xec, 0xe9, 0xea,
                0xcb, 0xc8, 0xcd, 0xce, 0xc7, 0xc4, 0xc1, 0xc2, 0xd3, 0xd0, 0xd5, 0xd6, 0xdf, 0xdc, 0xd9, 0xda,
                0x5b, 0x58, 0x5d, 0x5e, 0x57, 0x54, 0x51, 0x52, 0x43, 0x40, 0x45, 0x46, 0x4f, 0x4c, 0x49, 0x4a,
                0x6b, 0x68, 0x6d, 0x6e, 0x67, 0x64, 0x61, 0x62, 0x73, 0x70, 0x75, 0x76, 0x7f, 0x7c, 0x79, 0x7a,
                0x3b, 0x38, 0x3d, 0x3e, 0x37, 0x34, 0x31, 0x32, 0x23, 0x20, 0x25, 0x26, 0x2f, 0x2c, 0x29, 0x2a,
                0x0b, 0x08, 0x0d, 0x0e, 0x07, 0x04, 0x01, 0x02, 0x13, 0x10, 0x15, 0x16, 0x1f, 0x1c, 0x19, 0x1a
        };

/** Rijndael key schedule
 * copied from : http://en.wikipedia.org/wiki/Rijndael_key_schedule
 */
unsigned char rcon[256] =
        {
                0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
                0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
                0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
                0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
                0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
                0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
                0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
                0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
                0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
                0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
                0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
                0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
                0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
                0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
                0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
                0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
        };

/** Procedure: https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
 * 1. Key Expansion
 * 2. Initial Round Key
 *
 * Repeat 3-6 9x for 128 bit keys (11x for 192 bits and 13x for 256 bits)
 * 3. Substitute Bytes
 * 4. Shift Rows
 * 5. Mix Columns
 * 6. Add Round Key
 *
 * Final Round
 * 7. Substitute Bytes
 * 8. Shift Rows
 * 9. Add Round Key
 */
void aes_encrypt(struct AES * aes){

    //Initialization
    key_expansion(aes);
    add_round_key(aes, 0);

    //9 intermediate rounds
    for(int x = 1; x < 10; x++){
        substitute_bytes(aes);
        shift_rows(aes);
        mix_columns(aes);
        add_round_key(aes, x);
    }

    //Final Round
    substitute_bytes(aes);
    shift_rows(aes);
    add_round_key(aes, 10);

    //printf("encrypted 1 block\n");
}


/**
 * Source:
 * https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
 * http://en.wikipedia.org/wiki/Rijndael_S-box
 *
 * Substitution of each byte from LUT in the S-box
 */
void substitute_bytes(struct AES * aes){
    for(int x = 0; x < 4; x++) {
        for(int y = 0; y < 4; y++) {
            aes->state[x][y] = sbox[aes->state[x][y]];
        }
    }
    //printf("substituted bytes\n");
}

/**
 * Source:
 * https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
 *
 * Rows of the state are shifted cyclically according to offset (row number)
 */
void shift_rows(struct AES * aes){

    // faster than loop
    uint8_t temp;

    // Row 1 << 1 col
    temp = aes->state[0][1];
    aes->state[0][1] = aes->state[1][1];
    aes->state[1][1] = aes->state[2][1];
    aes->state[2][1] = aes->state[3][1];
    aes->state[3][1] = temp;

    // Row 2 << 2 col
    temp = aes->state[0][2];
    aes->state[0][2] = aes->state[2][2];
    aes->state[2][2] = temp;
    temp = aes->state[1][2];
    aes->state[1][2] = aes->state[3][2];
    aes->state[3][2] = temp;

    // Row 3 << 3 col
    temp = aes->state[0][3];
    aes->state[0][3] = aes->state[3][3];
    aes->state[3][3] = aes->state[2][3];
    aes->state[2][3] = aes->state[1][3];
    aes->state[1][3] = temp;


//    for(int x = 0; x < 4; x++) {
//        for(int y = 1; y < 4; y++) {
//            aes->state[x][y] = aes->state[x+y%4][y]];
//        }
//    }

    //printf("shifted rows\n");
}

/**
 * Source:
 * https://en.wikipedia.org/wiki/Rijndael_MixColumns
 *
 * Multiply each column by the transform matrix below in the Rijndael galois field:
 *
 * [ 2 3 1 1 ]
 * [ 1 2 3 1 ]
 * [ 1 1 2 3 ]
 * [ 3 1 1 2 ]
 */
void mix_columns(struct AES * aes){
    byte temp_state[4][4];
    for(int i = 0; i < 4; i++){
        
        // multiply by [ 2 3 1 1 ]
        temp_state[i][0] = galois_2[aes->state[i][0]] ^ galois_3[aes->state[i][1]] ^ aes->state[i][2] ^ aes->state[i][3];

        // multiply by [ 1 2 3 1 ]
        temp_state[i][1] = aes->state[i][0] ^ galois_2[aes->state[i][1]] ^ galois_3[aes->state[i][2]] ^ aes->state[i][3];

        // multiply by [ 1 1 2 3 ]
        temp_state[i][2] = aes->state[i][0] ^ aes->state[i][1] ^ galois_2[aes->state[i][2]] ^ galois_3[aes->state[i][3]];

        // multiply by [ 3 1 1 2 ]
        temp_state[i][3] = galois_3[aes->state[i][0]] ^ aes->state[i][1] ^ aes->state[i][2] ^ galois_2[aes->state[i][3]];

    }
    memcpy(aes->state, temp_state, 16);
    //printf("mixed columns\n");
}
/**
 * Source:
 * https://en.wikipedia.org/wiki/AES_key_schedule
 *
 * XOR the state array with the part of the expanded key for the current round
 */
void add_round_key(struct AES * aes, int round_number){
    for(int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            aes->state[x][y] ^= aes->expanded_key[(round_number << 2) + x][y];
        }
    }
    //printf("added round key\n");
}

/** Expand key to 128 bits using Rijndael's key schedule
 * https://en.wikipedia.org/wiki/AES_key_schedule
 *
 * Also very helpful:
 * https://engineering.purdue.edu/kak/compsec/NewLectures/Lecture8.pdf
 */
void key_expansion(struct AES * aes) {

    int iter = 1;

    // first 4 bytes of expanded key are
    memcpy(aes->expanded_key, aes->key, 16);

    //need to fill remaining 40
    for (int r = 4; r < 44; r++) {

        byte tmp[4];

        // copy previous 4 bytes of expanded_key to temp
        memcpy(tmp, aes->expanded_key[r - 1], 4);

        // Apply core for every 16 bytes
        if (r % 4 == 0) {
            key_schedule(tmp, iter++);
        }

        // XOR with the 4-byte block 16 bytes before current index
        for (int c = 0; c < 4; c++) {
            aes->expanded_key[r][c] = aes->expanded_key[r - 4][c] ^ tmp[c];
        }

    }

}
/**
  * Implementation of Rijndael key schedule.
  * http://en.wikipedia.org/wiki/Rijndael_key_schedule
  *
  * http://en.wikipedia.org/wiki/Rijndael_S-box
  * The Rijndael key schedule uses the Rijndael substitution box.
  *
  * http://en.wikipedia.org/wiki/Rijndael_key_schedule#Rcon
  * Rcon is the exponentiation of 2 to a specific value which is performed in Rijndael's finite field, GF(2^8).
  * The LUT "rcon" is used instead of performing the calculations.
  */
void key_schedule(byte row[4], int iter){

    //rotates a 32-bit word by 8 bits
    byte temp = row[0];
    for(int x = 1; x < 4; x++){
        row[x-1] = row[x];
    }
    row[3] = temp;

    //perform sbox transformation
    for(int y = 0; y < 4; y++) {
        row[y] = sbox[row[y]];
    }

    //XOR with rcon value
    row[0] = row[0] ^ rcon[iter];

    //printf("expanded key\n");
}

void aes_decrypt(struct AES * aes){
    printf("decrypted");
}

#include "aes.h"
#include "stdio.h"
#include <string.h>
#include <stdint.h>


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
    initial_round_key(aes);

    //9 intermediate rounds
    for(int i = 1; i < 10; i++){
        substitute_bytes(aes);
        shift_rows(aes);
        mix_columns(aes);
        add_round_key(aes);
    }

    //Final Round
    substitute_bytes(aes);
    shift_rows(aes);
    add_round_key(aes);

    printf("encrypted 1 block\n");
}


void initial_round_key(struct AES * aes){
    printf("initial key added\n");
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
    printf("substituted bytes\n");
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

    printf("shifted rows\n");
}

/**
 * Source:
 * https://en.wikipedia.org/wiki/Rijndael_MixColumns
 *
 *
 */
void mix_columns(struct AES * aes){
    printf("mixed columns\n");
}

void add_round_key(struct AES * aes){
    printf("added round key\n");
}

/** Expand key to 128 bits using Rijndael's key schedule
 * https://en.wikipedia.org/wiki/AES_key_schedule
 */
void key_expansion(struct AES * aes){

    //first 4 bytes of expanded key are first 4 bytes of initial key
    memcpy(aes->expanded_key, aes->key, 16);

    printf("expanded key\n");
}

void aes_decrypt(struct AES * aes){
    printf("decrypted");
}

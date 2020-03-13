#ifndef AES_H
#define AES_H

#include "string.h"

typedef unsigned char byte;

struct AES {
    unsigned char key[4][4];
    unsigned char expanded_key[44][4];
    unsigned char state[4][4];
};

//general structures for encrypting/decrypting
void aes_encrypt(struct AES * aes);
void aes_decrypt(struct AES * aes);

//key operations
void key_expansion(struct AES * aes);
void add_round_key(struct AES * aes, int round);

//encrypting operations
void substitute_bytes(struct AES * aes);
void shift_rows(struct AES * aes);
void mix_columns(struct AES * aes);
void key_schedule(byte row[4], int iter);

//decrypting operations
void inv_substitute_bytes(struct AES * aes);
void inv_shift_rows(struct AES * aes);
void inv_mix_columns(struct AES * aes);
void inv_add_round_key(struct AES * aes, int round);

#endif

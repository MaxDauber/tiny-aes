#ifndef AES_H
#define AES_H

#include "string.h"

struct AES {
    unsigned char key[4][4];
    unsigned char expanded_key[44][4];
    unsigned char state[4][4];
};

void aes_encrypt(struct AES * aes);
void aes_decrypt(struct AES * aes);
void key_expansion(struct AES * aes);
void initial_round_key(struct AES * aes);
void substitute_bytes(struct AES * aes);
void shift_rows(struct AES * aes);
void mix_columns(struct AES * aes);
void add_round_key(struct AES * aes);

#endif

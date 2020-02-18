#ifndef AES_H
#define AES_H

#include "string.h"
typedef unsigned char byte;

struct AES {
    byte key[4][4];
    byte expanded_key[44][4];
    byte state[4][4];
};

void aes_encrypt(struct AES * aes);
void aes_decrypt(struct AES * aes);

#endif

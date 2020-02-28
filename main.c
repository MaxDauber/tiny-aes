#ifndef AES_MAIN
#define AES_MAIN

#include "aes.h"
#include "stdio.h"
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[]) {

    //create struct and read key
    struct AES aes_struct;

    //read in key
    fread(aes_struct.key, 4, 4, stdin);

    //read in next block (guaranteed n < 10^6)
    while(!feof(stdin)) {
        fread(aes_struct.state, 4, 4, stdin);

        //encrypt block and output to stdout
        if(!feof(stdin)) {
            aes_encrypt(&aes_struct);
            fwrite(aes_struct.state, 4, 4, stdout);
        }
    }

    return 0;

}

#endif

#ifndef AES_MAIN
#define AES_MAIN

#include "aes.h"
#include "stdio.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fptr = fopen("aes_sample.in","rb");

    //create struct and read key
    struct AES aes_struct;
    fread(aes_struct.key, 4, 4, fptr);
    fread(aes_struct.state, 4, 4, fptr);
    aes_encrypt(&aes_struct);
    fwrite(aes_struct.state, 4, 4, stdout);
//    //read in next block (guaranteed n < 10^6)
//    while(!feof(stdin)) {
//        fread(aes_struct.state, 4, 4, fptr);
//
//        //encrypt block and output to stdout
//        if(!feof(stdin)) {
//            aes_encrypt(&aes_struct);
//            fwrite(aes_struct.state, 4, 4, stdout);
//        }
//    }
    return 0;
}

#endif

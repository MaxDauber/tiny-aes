#ifndef AES_MAIN
#define AES_MAIN

#include "aes.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	FILE * stdin = fopen(argv[1], "r");

	unsigned char key_block[4][4];
	fread(key_block, 4, 4, stdin);
	while(!feof(stdin)) {
		fread(key_block, 4, 4, stdin);
		if(!feof(stdin)) {
			printf("%s\n", &key_block);
		}
	}
	return 0;
}

#endif

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BUFFER_SIZE 16
#define HEADER_SIZE 54 //bytes
#define IMAGE_SIZE 960*160*4 //bytes

int main() {
    uint8_t* p;
    uint8_t* rgba; //uint8_t rgba[IMAGE_SIZE]; //RGBA array
    
    uint8_t* gray; //uint8_t gray[IMAGE_SIZE / 4]; //Grayscale array
    size_t i, j;
    p = 0x40000000 + HEADER_SIZE; //memory pointer
    rgba = 0x40200000;
    gray = 0x40400000;
    //0x40200000, 0x407FFFFF Memory Map needed


    for (i = 0; i < IMAGE_SIZE; i++) {
        rgba[i] = p[i];
    }

    j = 0;
    while (j < IMAGE_SIZE / 4) {
        gray[j++] = ((rgba[j * 4]) + (rgba[j * 4 + 1]) + (rgba[j * 4 + 2])) / 3;
    }

    printf("Output.\n");


    _sys_exit(0);
}

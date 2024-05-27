#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BUFFER_SIZE 16
#define HEADER_SIZE 54 //bytes
#define IMAGE_SIZE 960*160*4 //bytes

int main() {
    uint8_t* p;
    
    uint8_t* gray; //uint8_t gray[IMAGE_SIZE / 4]; //Grayscale array
    size_t j;
    p = 0x40000000 + HEADER_SIZE; //memory pointer
    
    gray = 0x40400000;
    //0x40200000, 0x407FFFFF Memory Map needed

    j = 0;
    while (j < HEADER_SIZE+(IMAGE_SIZE / 4)) {
        gray[j++] = ((p[j * 4]) + (p[j * 4 + 1]) + (p[j * 4 + 2])) / 3;
    }

    printf("Output.\n");


    _sys_exit(0);
}

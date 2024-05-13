//last-update 0508-2240
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BUFFER_SIZE 16
#define HEADER_SIZE 54 //bytes
#define IMAGE_SIZE 960*160*4 //bytes

int main() {
    uint8_t *p;
    uint8_t *rgba; //uint8_t rgba[IMAGE_SIZE]; //RGBA array
    uint8_t *rgb; //uint8_t rgb[IMAGE_SIZE * 3/4]; //RGB array
    uint8_t *rgbcomp; //uint8_t rgbcomp[IMAGE_SIZE / 4]; //compressed RGB
    p = 0x40000000; //memory pointer - size 4000byte
    rgba = 0x40200000;
    rgb = 0x40400000;
    rgbcomp = 0x40600000;
    //0x40200000, 0x407FFFFF Memory Map needed
    size_t i = 0;
    size_t j = 0; 

    p = 0x40000000 + HEADER_SIZE;   //fseek(fileIn, HEADER_SIZE-1, SEEK_SET); //set pointer after header
    for(i = 0 ; i<IMAGE_SIZE; i++){
        rgba[i] = p[i];
    } //fread(rgba, sizeof(uint8_t), IMAGE_SIZE, fileIn); //Whole rgba read
    
    for (i = 0; i < IMAGE_SIZE; i += 4) {
        rgb[j++] = rgba[i];     // Red
        rgb[j++] = rgba[i + 1]; // Green
        rgb[j++] = rgba[i + 2]; // Blue
        // Alpha channel rgba[i + 3] is ignored
    } // Convert RGBA to RGB by ignoring the alpha channel
    
    unsigned char mask = 0b11100000; //R[7:5], G[7:5] masking
    j = 0;
    while (j < IMAGE_SIZE/4) {
        rgbcomp[j++] = (rgb[j*3] & mask) + ((rgb[j*3+1] & mask) >> 3) + ((rgb[j*3+2] & mask) >> 6);
    }

    printf("Output 'output_rgbcomp.bmp' created.\n");

    _sys_exit(0);
}

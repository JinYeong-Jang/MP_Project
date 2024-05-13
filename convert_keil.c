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
    p = 0x40000000; //memory pointer - size 4000byte
    size_t i = 0;
    size_t j = 0;
    uint8_t rgba[IMAGE_SIZE]; //RGBA array
    uint8_t rgb[IMAGE_SIZE / 4 * 3]; //RGB array
    uint8_t rgbcomp[IMAGE_SIZE / 4]; //compressed RGB
    uint8_t buffer[BUFFER_SIZE];    //16bytes size byffer

    p = 0x40000000 + HEADER_SIZE;   //fseek(fileIn, HEADER_SIZE-1, SEEK_SET); //set pointer after header
    for(i = 0 ; i<IMAGE_SIZE; i++){
        rgba[i] = *p;
        p += 0x1;
    } //fread(rgba, sizeof(uint8_t), IMAGE_SIZE, fileIn); //Whole rgba read
    
    for (i = 0; i < IMAGE_SIZE; i += 4) {
        rgb[j++] = rgba[i];     // Red
        rgb[j++] = rgba[i + 1]; // Green
        rgb[j++] = rgba[i + 2]; // Blue
        // Alpha channel rgba[i + 3] is ignored
    }
    // Convert RGBA to RGB by ignoring the alpha channel
    
    j = 0;
    unsigned char mask = 0b11100000; //R[7:5], G[7:5] masking
    unsigned char bmask = 0b11000000; //B[7:6] masking
    while (j < IMAGE_SIZE/4) {
        rgbcomp[j++] = (rgb[j*3] & mask) + ((rgb[j*3+1] & mask) >> 3) + ((rgb[j*3+2] & bmask) >> 6);
    }

    p = 0x41000000;
    for(i = 0; i < IMAGE_SIZE / 4; i++){
        *p=rgbcomp[i];
        p += 0x1;
    }

    printf("Output 'output_rgbcomp.bmp' created.\n");

    return 0;
}

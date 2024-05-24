//last-update 0520-2240
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BUFFER_SIZE 16
#define HEADER_SIZE 54 //bytes
#define IMAGE_SIZE 960*160*4 //bytes
#define COLOR_SIZE 960*160 //bytes

int main() {
    uint8_t *p;
    uint8_t *rgba; //uint8_t rgba[IMAGE_SIZE]; //RGBA array
    uint8_t *rgb; //uint8_t rgb[IMAGE_SIZE * 3/4]; //RGB array
    uint32_t *rgbcomp; //uint8_t rgbcomp[IMAGE_SIZE / 4]; //compressed RGB
    size_t i, j;
    p = 0x40000000 + HEADER_SIZE; //memory pointer - size 4000byte
    //fseek(fileIn, HEADER_SIZE-1, SEEK_SET); //set pointer after header
    rgba = 0x40200000;
    rgb = 0x40400000;
    rgbcomp = 0x40600000;
    //0x40200000, 0x407FFFFF Memory Map needed

    for(i = 0 ; i<IMAGE_SIZE; i++){
        rgba[i] = p[i];
    } //fread(rgba, sizeof(uint8_t), IMAGE_SIZE, fileIn); //Whole rgba read
    
    for (i = 0, j = 0; i < IMAGE_SIZE; i += 4, j++) {
        rgb[j] = rgba[i];   // Red
        rgb[j+COLOR_SIZE] = rgba[i + 1]; // Green
        rgb[j+COLOR_SIZE*2] = rgba[i + 2]; // Blue
        // Alpha channel rgba[i + 3] is ignored
    } // Convert RGBA to RGB by ignoring the alpha channel, RRRGGGBBB order

    uint32_t mask = 0xE0E0E0E0; //masking 0b1110_0000 by register length
    uint32_t *readbuffer;
    readbuffer = 0x40400000;
    i = 0;
	j = 0;
    while (j < COLOR_SIZE/4) {
        rgbcomp[j++] += readbuffer[i++]&mask;
    }
    j=0;
    while (j < COLOR_SIZE/4) {
        rgbcomp[j++] += (readbuffer[i++]&mask) >> 3;
    }
    mask -= 0x20202020; //0xE0E0E0E0 to 0xC0C0C0C0
    j=0;
    while (j < COLOR_SIZE/4) {
        rgbcomp[j++] += (readbuffer[i++]&mask) >> 6;
    }
    printf("Output 'output_rgbcomp.bmp' created.\n");

    _sys_exit(0);
}

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
	uint8_t *rgb;
    uint8_t* gray; //uint8_t gray[IMAGE_SIZE / 4]; //Grayscale array
    size_t i, j;
    p = 0x40000000 + HEADER_SIZE; //memory pointer
    rgba = 0x40200000;
	rgb = 0x40400000;
    gray = 0x40600000;
    //0x40200000, 0x407FFFFF Memory Map needed

    
    for (i = 0; i < IMAGE_SIZE; i++) {
        rgba[i] = p[i];
    } 

    
   for (i = 0, j = 0; i < IMAGE_SIZE; i += 4) {
        rgb[j++] = rgba[i];     // Red
        rgb[j++] = rgba[i + 1]; // Green
        rgb[j++] = rgba[i + 2]; // Blue
        // Alpha channel rgba[i + 3] is ignored
    } // Convert RGBA to RGB by ignoring the alpha channel

		for (i = 0; i < IMAGE_SIZE/4; i++) {
        uint8_t red = rgb[i*3];
        uint8_t green = rgb[i*3 + 1];
        uint8_t blue = rgb[i*3 + 2];
        
        gray[i] = (red + green + blue) / 3; // Store the grayscale value
    }
   
    printf("Output.\n");

    _sys_exit(0);
}

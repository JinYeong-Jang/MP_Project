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
    p = (uint8_t*)0x40000000 + HEADER_SIZE; //memory pointer
    rgba = (uint8_t*)0x40200000;
	  rgb = 0x40400000;
    gray = (uint8_t*)0x40600000;
    //0x40200000, 0x407FFFFF Memory Map needed

    // RGBA µ¥ÀÌÅÍ¸¦ ÀÐ¾î¿É´Ï´Ù.
    for (i = 0; i < IMAGE_SIZE; i++) {
        rgba[i] = p[i];
    } // ¿¹½Ã¿¡¼­´Â ÆÄÀÏ ÀÔÃâ·Â ´ë½Å ¸Þ¸ð¸®¿¡¼­ Á÷Á¢ ÀÐ¾î¿É´Ï´Ù.

    
   for (i = 0, j = 0; i < IMAGE_SIZE; i += 4) {
        rgb[j++] = rgba[i];     // Red
        rgb[j++] = rgba[i + 1]; // Green
        rgb[j++] = rgba[i + 2]; // Blue
        // Alpha channel rgba[i + 3] is ignored
    } // Convert RGBA to RGB by ignoring the alpha channel

		for (i = 0, j = 0; i < IMAGE_SIZE; i += 4) {
        uint8_t r = rgb[i];     // Red
        uint8_t g = rgb[i + 1]; // Green
        uint8_t b = rgb[i + 2]; // Blue
        
        gray[j++] = (r + g + b) /3;
    }
    printf("Output 'output_gray.bmp' created.\n");

    // ÇÁ·Î±×·¥ Á¾·á
    _sys_exit(0);
}

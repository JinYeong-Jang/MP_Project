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
    size_t k = 0;

    p = 0x40000000 + HEADER_SIZE;   //fseek(fileIn, HEADER_SIZE-1, SEEK_SET); //set pointer after header
    for(i = 0 ; i<IMAGE_SIZE; i++){
        rgba[i] = p[i];
    } //fread(rgba, sizeof(uint8_t), IMAGE_SIZE, fileIn); //Whole rgba read
    
    for (k = 0; k < 3; k++){ //R, G, B  *3 times
        for (i = 0; i < IMAGE_SIZE; i += 4) {
            rgb[j++] = rgba[i+k];     // RRRGGGBBB order
        } // Convert RGBA to RGB by ignoring the alpha channel
    }

    uint32_t mask = 0xE0E0E0E0; //masking 0b1110_0000 by register length
	uint32_t bmask = 0xC0C0C0C0;
    uint32_t *regtmp;
    uint32_t *readbuffer;
    readbuffer = 0x40400000;
    regtmp = 0x40600000;
    i = 0;
	j=0;
    while (j < IMAGE_SIZE/16) {
        regtmp[j++] += readbuffer[i++]&mask;
    }
    j=0;
    while (j < IMAGE_SIZE/16) {
        regtmp[j++] += (readbuffer[i++]&mask) >> 3;
    }
    j=0;
    while (j < IMAGE_SIZE/16) {
        regtmp[j++] += (readbuffer[i++]&bmask) >> 6;
    }
	/* //위의 3개의 while문과 같은 코드
	for(k = 0; k < 3; k++){
        j = 0;
        while (j < IMAGE_SIZE/16 && k < 2) {
            regtmp[j++] += readbuffer[i++]&mask >> k*3;
        }
				while (j < IMAGE_SIZE/16 && k == 2) {
            regtmp[j++] += readbuffer[i++]&bmask >> 6;
        }
    }
	*/
    printf("Output 'output_rgbcomp.bmp' created.\n");

    _sys_exit(0);
}
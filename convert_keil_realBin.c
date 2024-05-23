//last-update 0523-1630
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define IMAGE_SIZE 960*640*4 //bytes, 0x25_8000
#define COLOR_SIZE 960*640 //bytes
#define BINARY_SIZE 960*640/8 //bytes, 0x1_2C00

int main() {
    uint8_t *rgba; //uint8_t rgba[IMAGE_SIZE]; //RGBA array
    uint8_t *rgb; //uint8_t rgb[IMAGE_SIZE * 3/4]; //RGB array
    uint32_t *rgbcomp; //uint8_t rgbcomp[IMAGE_SIZE / 4]; //compressed RGB
    uint32_t *readbuffer; //same as rgb[] but access in 32bit
    size_t i, j, k;
    rgba = 0x50000000;
    rgb = rgba+IMAGE_SIZE;
    rgbcomp = rgb+IMAGE_SIZE;
    readbuffer = rgba+IMAGE_SIZE;
    //0x50000000, 0x50720000 Memory Map needed
    
    for (i = 0, j = 0; i < IMAGE_SIZE; i += 4, j++) {
        rgb[j] = rgba[i];   // Red
        rgb[j+COLOR_SIZE] = rgba[i + 1]; // Green
        rgb[j+COLOR_SIZE*2] = rgba[i + 2]; // Blue
        // Alpha channel rgba[i + 3] is ignored
    } // Convert RGBA to RGB by ignoring the alpha channel while divide into 3, RRRGGGBBB order

    rgb = 0x50000000+IMAGE_SIZE; //reset pointer
    for(i=0; i<COLOR_SIZE*3; i++){
        rgb[i] = rgb[i]/3;
    }

    i = 0;
    for(k = 0; k < 3; k++){
        j = 0;
        while (j < COLOR_SIZE/4) {
            rgbcomp[j++] += readbuffer[i++];
        }
    }

    rgba = 0x504B0000; //reuse pointer - target to rgbcomp
    rgb = 0x50708000; //reuse pointer - target to bin
    j = 0; k = 0;
    uint8_t binBuffer;
    while (j< BINARY_SIZE) {
        for(i = 7; i > 0; i--){
            if (rgba[k++] > 0x7F){ //127
                binBuffer += 1 << i;
            } /*else {
                binBuffer += 0
            }*/
        }
        rgb[j++] = binBuffer;
        binBuffer = 0;
    }
    

    printf("Convertion done.\n");

    _sys_exit(0);
}

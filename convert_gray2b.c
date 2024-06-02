#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BUFFER_SIZE 16
#define HEADER_SIZE 54 //bytes
#define IMAGE_SIZE 960*160*4 //bytes
#define COLOR_SIZE 960*160 //bytes

int main() {
    uint8_t* p;
    uint8_t* rgba; //uint8_t rgba[IMAGE_SIZE]; //RGBA array
    uint8_t* rgb; //uint8_t rgb[IMAGE_SIZE * 3/4]; //RGB array
    uint8_t* grayscale; //uint8_t grayscale[COLOR_SIZE]; //Grayscale array
    size_t i, j;
    p = (uint8_t*)0x40000000 + HEADER_SIZE; //memory pointer - size 4000byte
    rgba = (uint8_t*)0x40200000;
    rgb = (uint8_t*)0x40400000;
    grayscale = (uint8_t*)0x40600000; // Assign memory for grayscale values

    // Memory Map: 0x40200000, 0x407FFFFF needed
    for (i = 0; i < IMAGE_SIZE; i++) {
        rgba[i] = p[i];
    } // Whole rgba read

    // Convert RGBA to RGB by ignoring the alpha channel
    for (i = 0, j = 0; i < IMAGE_SIZE; i += 4, j++) {
        rgb[j] = rgba[i];   // Red
        rgb[j + COLOR_SIZE] = rgba[i + 1]; // Green
        rgb[j + COLOR_SIZE * 2] = rgba[i + 2]; // Blue
        // Alpha channel rgba[i + 3] is ignored
    }

    for (i = 0, j=0; i < COLOR_SIZE; i++) {
        uint8_t red = rgb[i];
        uint8_t green = rgb[i + COLOR_SIZE];
        uint8_t blue = rgb[i + COLOR_SIZE * 2];

        // Find the maximum and minimum of the RGB values
        uint8_t max_val = (red > green) ? (red > blue ? red : blue) : (green > blue ? green : blue);
        uint8_t min_val = (red < green) ? (red < blue ? red : blue) : (green < blue ? green : blue);

        // Calculate the average of the max and min values
        grayscale[j++] = (min_val + max_val) / 2;
    }

    printf("OUT.\n");

    _sys_exit(0);
}

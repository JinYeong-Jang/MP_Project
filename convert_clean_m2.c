//last-update 0509-2240
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BUFFER_SIZE 16
#define HEADER_SIZE 55 //bytes
#define IMAGE_SIZE 960*160*4 //bytes

int main() {
    uint8_t rgba[IMAGE_SIZE]; //RGBA array
    uint8_t rgb[IMAGE_SIZE / 4 * 3]; //RGB array
    uint8_t rgbcomp[IMAGE_SIZE / 4]; //compressed RGB
    uint8_t buffer[BUFFER_SIZE];    //16bytes size buffer
    size_t bytes_read;              //var for bytes read
    FILE *fileIn = fopen("2024S_MA_image_rgba.png", "rb");
    FILE *fileOut = fopen("output_rgb_m2.png", "wb");
    FILE *fileOut2 = fopen("output_rgbcomp_m2.png", "wb");

    //FILE fetch failed
    if (!fileIn) {
        printf("Failed to open input file.\n");
        return 0;
    }
    if (!fileOut2) {
        printf("Failed to open output file.\n");
        fclose(fileIn);
        return 0;
    }
    //If any data readed
    bytes_read = fread(buffer, sizeof(uint8_t), IMAGE_SIZE, fileIn);
    if (bytes_read == 0) {
        fprintf(stderr, "No data read from file.\n");
        fclose(fileIn);
        fclose(fileOut2);
        return 1;
    }

    fseek(fileIn, HEADER_SIZE-1, SEEK_SET); //set pointer after header
    fread(rgba, sizeof(uint8_t), IMAGE_SIZE, fileIn); //all rgba read
    // Convert RGBA to RGB by ignoring the alpha channel
    size_t j = 0;
    for(size_t k = 0; k < 3; k++){ //k로 반복 추가
        for (size_t i = 0; i < bytes_read; i += 4) {
            rgb[j++] = rgba[i+k];     // Red
            // rgb[j++] = rgba[i + 1]; // Green
            // rgb[j++] = rgba[i + 2]; // Blue
            // Alpha channel rgba[i + 3] is ignored
        }
    }
    unsigned char mask = 0b11100000; //R[7:5], G[7:5] masking
    // unsigned char bmask = 0b11000000; //B[7:6] masking
    j = 0;
    size_t i = 0;
    uint8_t tmp;
    for(size_t k = 0; k < 3; k++){
        i = 0;
        while (i < IMAGE_SIZE/4) {
            tmp = (rgb[j++] & mask) >> k*3;
            rgbcomp[i++] += tmp;
            // rgbcomp[j++] = (rgb[j*3] & mask) + ((rgb[j*3+1] & mask) >> 3) + ((rgb[j*3+2] & bmask) >> 6);
        }
    }
    


    // Write the RGB data to the output file
    fwrite(rgb, 1, IMAGE_SIZE / 4 * 3, fileOut);
    fwrite(rgbcomp, 1, IMAGE_SIZE / 4, fileOut2);

    // Close the files
    fclose(fileIn);
    fclose(fileOut);
    fclose(fileOut2);

    printf("Output 'output_rgbcomp_m2.bmp' created.\n");

    return 0;
}

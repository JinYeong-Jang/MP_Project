//last-update 0508-2240
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BUFFER_SIZE 16
#define HEADER_SIZE 55 //bytes
#define IMAGE_SIZE 960*160*4 //bytes

int main() {
    unsigned char rgba[IMAGE_SIZE]; //RGBA배열
    unsigned char rgb[IMAGE_SIZE / 4 * 3]; //RGB배열
    unsigned char rgbcomp[IMAGE_SIZE / 4]; //compressed RGB
    uint8_t buffer[BUFFER_SIZE];    //16bytes크기 버퍼
    size_t bytes_read;              //읽은 바이트 수
    FILE *fileIn = fopen("2024S_MA_image_rgba.png", "rb");
    FILE *fileOut = fopen("output_rgb.png", "wb");
    FILE *fileOut2 = fopen("output_rgbcomp.png", "wb");

    //파일 입출력 실패시
    if (!fileIn) {
        printf("Failed to open input file.\n");
        return 0;
    }
    if (!fileOut) {
        printf("Failed to open output file.\n");
        fclose(fileIn);
        return 0;
    }
    //읽은 데이터가 없는 경우
    bytes_read = fread(buffer, sizeof(uint8_t), IMAGE_SIZE, fileIn);
    if (bytes_read == 0) {
        fprintf(stderr, "No data read from file.\n");
        fclose(fileIn);
        fclose(fileOut);
        return 1;
    }

    fseek(fileIn, HEADER_SIZE-1, SEEK_SET); //파일 포인터를 헤더 이후로 위치
    fread(rgba, sizeof(uint8_t), IMAGE_SIZE, fileIn); //전체 rgba read
    // Convert RGBA to RGB by ignoring the alpha channel
    size_t j = 0;
    for (size_t i = 0; i < bytes_read; i += 4) {
        rgb[j++] = rgba[i];     // Red
        rgb[j++] = rgba[i + 1]; // Green
        rgb[j++] = rgba[i + 2]; // Blue
        // Alpha channel rgba[i + 3] is ignored
    }
    j = 0;
    unsigned char mask = 0b11100000; //R[7:5], G[7:5] 구현
    unsigned char bmask = 0b11000000; //B[7:6] 구현
    while (j < IMAGE_SIZE/4) {
        unsigned char r = rgb[j*3] & mask;
        unsigned char g = (rgb[j*3+1] & mask) >> 3;
        unsigned char b = (rgb[j*3+2] & bmask) >> 6;
        rgbcomp[j++] = r+g+b;
    }


    // Write the RGB data to the output file
    fwrite(rgb, 1, IMAGE_SIZE / 4 * 3, fileOut);
    fwrite(rgbcomp, 1, IMAGE_SIZE / 4, fileOut2);

    // Close the files
    fclose(fileIn);
    fclose(fileOut);
    fclose(fileOut2);

    printf("Output 'output_rgb.bmp', 'output_rgbcomp.bmp' created.\n");

    return 0;
}
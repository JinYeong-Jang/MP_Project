#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define IMAGE_SIZE 960*160*4 //bytes

int main() {
    uint8_t rgba[IMAGE_SIZE]; // RGBA array
    uint8_t grayscale[IMAGE_SIZE / 4]; // Grayscale array
    size_t bytes_read; // var for bytes read
    FILE* fileIn = fopen("2024S_MA_image_rgba.png", "rb");
    FILE* fileOut = fopen("output_grayscale2.png", "wb");

    // FILE fetch failed
    if (!fileIn) {
        printf("Failed to open input file.\n");
        return 0;
    }
    if (!fileOut) {
        printf("Failed to open output file.\n");
        fclose(fileIn);
        return 0;
    }

    // Read the entire RGBA data
    bytes_read = fread(rgba, sizeof(uint8_t), IMAGE_SIZE, fileIn);
    if (bytes_read == 0) {
        fprintf(stderr, "No data read from file.\n");
        fclose(fileIn);
        fclose(fileOut);
        return 1;
    }

    fwrite(rgba, 1, 54, fileOut);

    // Convert RGBA to Grayscale
    for (size_t i = 54, j = 54 / 4; i < bytes_read; i += 4, j++) {
        uint8_t r = rgba[i];
        uint8_t g = rgba[i + 1];
        uint8_t b = rgba[i + 2];
        // Use weighted average method for grayscale
        uint8_t max = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
        uint8_t min = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);
        grayscale[j] = (uint8_t)((max + min) / 2);
    }

    // Write the Grayscale data to the output file
    fwrite(grayscale, 1, (bytes_read - 54) / 4, fileOut);

    // Close the files
    fclose(fileIn);
    fclose(fileOut);

    printf("그레이스케일 변환 파일 'output_grayscale2.png' 생성 완료.\n");

    return 0;
}


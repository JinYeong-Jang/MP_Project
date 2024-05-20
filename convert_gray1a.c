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
    uint8_t* gray; //uint8_t gray[IMAGE_SIZE / 4]; //Grayscale array
    size_t i, j;
    p = (uint8_t*)0x40000000 + HEADER_SIZE; //memory pointer
    rgba = (uint8_t*)0x40200000;
    gray = (uint8_t*)0x40400000;
    //0x40200000, 0x407FFFFF Memory Map needed

    // RGBA 데이터를 읽어옵니다.
    for (i = 0; i < IMAGE_SIZE; i++) {
        rgba[i] = p[i];
    } // 예시에서는 파일 입출력 대신 메모리에서 직접 읽어옵니다.

    // RGBA를 그레이스케일로 변환합니다.
    for (i = 0, j = 0; i < IMAGE_SIZE; i += 4) {
        uint8_t r = rgba[i];     // Red
        uint8_t g = rgba[i + 1]; // Green
        uint8_t b = rgba[i + 2]; // Blue
        // 그레이스케일 값은 RGB 값의 평균으로 계산됩니다.
        gray[j++] = (r + g + b) / 3;
    }

    printf("Output 'output_gray.bmp' created.\n");

    // 프로그램 종료
    _sys_exit(0);
}

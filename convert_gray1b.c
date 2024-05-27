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
    uint8_t* rgb;
    uint8_t* gray; //uint8_t gray[IMAGE_SIZE / 4]; //Grayscale array
    size_t i, j;
    uint8_t min_val, max_val;
    p = 0x40000000 + HEADER_SIZE; //memory pointer
    rgba = 0x40200000;
    rgb = 0x40400000;
    gray = 0x40600000;
    //0x40200000, 0x407FFFFF Memory Map needed

    // RGBA �����͸� �о�ɴϴ�.
    for (i = 0; i < IMAGE_SIZE; i++) {
        rgba[i] = p[i];
    } // ���ÿ����� ���� ����� ��� �޸𸮿��� ���� �о�ɴϴ�.
    
    for (i = 0, j = 0; i < IMAGE_SIZE; i += 4) {
        rgb[j++] = rgba[i];     // Red
        rgb[j++] = rgba[i + 1]; // Green
        rgb[j++] = rgba[i + 2]; // Blue
        // Alpha channel rgba[i + 3] is ignored
    } // Convert RGBA to RGB by ignoring the alpha channel

    j = 0;
    while (j < IMAGE_SIZE / 4) {
        min_val = (rgb[j * 3]) < (rgb[j * 3 + 1]) ? ((rgb[j * 3]) < (rgb[j * 3 + 2]) ? (rgb[j * 3]) : (rgb[j * 3 + 2])) : ((rgb[j * 3 + 1]) < (rgb[j * 3 + 2]) ? (rgb[j * 3 + 1]) : (rgb[j * 3 + 2]));
        max_val = (rgb[j * 3]) > (rgb[j * 3 + 1]) ? ((rgb[j * 3]) > (rgb[j * 3 + 2]) ? (rgb[j * 3]) : (rgb[j * 3 + 2])) : ((rgb[j * 3 + 1]) > (rgb[j * 3 + 2]) ? (rgb[j * 3 + 1]) : (rgb[j * 3 + 2]));
        gray[j++] = (min_val + max_val) / 2;
    }

    /* RGBA�� �׷��̽����Ϸ� ��ȯ�մϴ�.
    for (i = 0, j = 0; i < IMAGE_SIZE; i += 3) {
        uint8_t r = rgb[i];     // Red
        uint8_t g = rgb[i + 1]; // Green
        uint8_t b = rgb[i + 2]; // Blue

        // RGB �� �ּҰ��� �ִ밪 ã��
        uint8_t min_val = r < g ? (r < b ? r : b) : (g < b ? g : b);
        uint8_t max_val = r > g ? (r > b ? r : b) : (g > b ? g : b);

        // �ּҰ��� �ִ밪�� ��� ���
        gray[j++] = (min_val + max_val) / 2;
    }*/

    printf('output.\n');

    // ���α׷� ����
    _sys_exit(0);
}

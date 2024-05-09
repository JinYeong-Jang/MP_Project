/*
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define SIZE 1000 // 불러올 16진수 데이터의 갯수

int main() 
{
    unsigned char png[SIZE];
    FILE* file = fopen("2024S_MA_image_rgba.png", "rb");

    if (!file || fread(png, 1, SIZE, file) != SIZE) {
        fprintf(stderr, "파일 읽기에 실패하였습니다.\n");
        return 1;
    }

    // 16진수 변환
    for (size_t i = 0; i < SIZE; i++)
        printf("%02X ", png[i]);
    printf("\n");

    fclose(file);
}
*/

#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 16
#define HEADER_SIZE 55 //bytes
#define IMAGE_SIZE 960*160 //bytes

int main(int argc, char *argv[])
{
    const char* filename = "2024S_MA_image_rgba.png";
    uint8_t buffer[BUFFER_SIZE];
    size_t bytes_read;
    int bytes_read_sum = 0;

    FILE *file = fopen(filename, "rb");
    if(!file){
        printf("Error opening file.\n");
        return 1;
    }

    printf("Binary data for file: %s\n", filename);
    while ((bytes_read = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, file)) > 0 && bytes_read_sum < HEADER_SIZE){
        for(size_t i = 0; i < bytes_read; i++){
            printf("%02X ", buffer[i]);
        }
        printf("\n");
        bytes_read_sum += bytes_read;
    }

    printf("\n\nData after header:\n");
    fseek(file, HEADER_SIZE-1, SEEK_SET);
    while ((bytes_read = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, file)) > 0 && bytes_read_sum < IMAGE_SIZE/960){
        for(size_t i = 0; i < bytes_read; i++){
            printf("%02X ", buffer[i]);
        }
        printf("\n");
        bytes_read_sum += bytes_read;
    }
    
    fclose(file);
    return 0;
}
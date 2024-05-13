#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


unsigned char convertToGrayscaleAverage(unsigned char r, unsigned char g, unsigned char b) {
    return (r + g + b) / 3;
}

int main() {
    const char* inputPath = "2024S_MA_image_rgba.hex"; // 입력 파일 경로
    const char* outputPath = "2024S_MA_image_rgba_out.hex"; // 출력 파일 경로

    FILE* inputFile;
    FILE* outputFile;
    

    // 파일 열기
    inputFile = fopen(inputPath, "rb");
    outputFile = fopen(outputPath, "wb");

    if (inputFile == NULL || outputFile == NULL) {
        perror("파일 열기 실패");
        // 열린 파일이 있다면 닫기
        if (inputFile != NULL) fclose(inputFile);
        if (outputFile != NULL) fclose(outputFile);
        return EXIT_FAILURE;
    }

    unsigned char buffer;
    unsigned char rgba[4];
    unsigned char grayscale;
    int foundIDAT = 0; // IDAT 시그니처를 찾았는지 여부

    // IDAT 시그니처 찾기 (49 44 41 54)
    /*while (fread(&buffer, 1, 1, inputFile) == 1) {
        if (buffer == 0x49) {
            fread(&buffer, 1, 1, inputFile);
            if (buffer == 0x44) {
                fread(&buffer, 1, 1, inputFile);
                if (buffer == 0x41) {
                    fread(&buffer, 1, 1, inputFile);
                    if (buffer == 0x54) {
                        foundIDAT = 1; // IDAT 발견
                        break;
                    }
                }
            }
        }
    }

    if (!foundIDAT) {
        printf("IDAT 시그니처를 찾을 수 없습니다.\n");
        fclose(inputFile);
        fclose(outputFile);
        return EXIT_FAILURE;
    }*/

    // IDAT 이후부터 처리 시작
    while (fread(rgba, 1, 4, inputFile) == 4) {
        // 그레이스케일 값 계산
        grayscale = convertToGrayscaleAverage(rgba[0], rgba[1], rgba[2]);

        // R, G, B에 그레이스케일 값 할당 (A 값은 변경하지 않음)
        rgba[0] = rgba[1] = rgba[2] = grayscale;

        // 결과를 출력 파일에 쓰기
        fwrite(rgba, 1, 4, outputFile);
    }

    // 파일 닫기
    fclose(inputFile);
    fclose(outputFile);

    return EXIT_SUCCESS;
}

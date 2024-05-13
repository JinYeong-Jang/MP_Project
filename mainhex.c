#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


unsigned char convertToGrayscaleAverage(unsigned char r, unsigned char g, unsigned char b) {
    return (r + g + b) / 3;
}

int main() {
    const char* inputPath = "2024S_MA_image_rgba.hex"; // �Է� ���� ���
    const char* outputPath = "2024S_MA_image_rgba_out.hex"; // ��� ���� ���

    FILE* inputFile;
    FILE* outputFile;
    

    // ���� ����
    inputFile = fopen(inputPath, "rb");
    outputFile = fopen(outputPath, "wb");

    if (inputFile == NULL || outputFile == NULL) {
        perror("���� ���� ����");
        // ���� ������ �ִٸ� �ݱ�
        if (inputFile != NULL) fclose(inputFile);
        if (outputFile != NULL) fclose(outputFile);
        return EXIT_FAILURE;
    }

    unsigned char buffer;
    unsigned char rgba[4];
    unsigned char grayscale;
    int foundIDAT = 0; // IDAT �ñ״�ó�� ã�Ҵ��� ����

    // IDAT �ñ״�ó ã�� (49 44 41 54)
    /*while (fread(&buffer, 1, 1, inputFile) == 1) {
        if (buffer == 0x49) {
            fread(&buffer, 1, 1, inputFile);
            if (buffer == 0x44) {
                fread(&buffer, 1, 1, inputFile);
                if (buffer == 0x41) {
                    fread(&buffer, 1, 1, inputFile);
                    if (buffer == 0x54) {
                        foundIDAT = 1; // IDAT �߰�
                        break;
                    }
                }
            }
        }
    }

    if (!foundIDAT) {
        printf("IDAT �ñ״�ó�� ã�� �� �����ϴ�.\n");
        fclose(inputFile);
        fclose(outputFile);
        return EXIT_FAILURE;
    }*/

    // IDAT ���ĺ��� ó�� ����
    while (fread(rgba, 1, 4, inputFile) == 4) {
        // �׷��̽����� �� ���
        grayscale = convertToGrayscaleAverage(rgba[0], rgba[1], rgba[2]);

        // R, G, B�� �׷��̽����� �� �Ҵ� (A ���� �������� ����)
        rgba[0] = rgba[1] = rgba[2] = grayscale;

        // ����� ��� ���Ͽ� ����
        fwrite(rgba, 1, 4, outputFile);
    }

    // ���� �ݱ�
    fclose(inputFile);
    fclose(outputFile);

    return EXIT_SUCCESS;
}

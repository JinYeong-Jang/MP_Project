#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
void SkipHeader(FILE* file, FILE* fileOut);
void IHDRChunk(FILE* file, FILE* fileOut, int width, int height, int bpp, int colortype);
void Decimaltohex(FILE* file, FILE* fileOut, int value);
int Square(int a, int b);
void FindIDATChunkAndCopyPaste(FILE* file, FILE* fileOut);
void RGBtoBinary(FILE* file, FILE* fileOut, int LSIZE);
void IENDChunk(FILE* fileOut);
const int IMAGE_SIZE = 960 * 160 * 3 ,ARRAY_SIZE = 960 * 160;
uint8_t R[ARRAY_SIZE];
uint8_t G[ARRAY_SIZE];
uint8_t B[ARRAY_SIZE];

int main()
{
    const int LSIZE = 153600; 
    FILE* file = fopen("output_rgb_m2.png", "rb");
    FILE* fileOut = fopen("output_binary_m2.png", "wb");
    SkipHeader(file, fileOut);
    IHDRChunk(file, fileOut, 960, 160, 1, 7);  
    FindIDATChunkAndCopyPaste(file, fileOut);
    RGBtoBinary(file, fileOut, LSIZE);
    IENDChunk(fileOut);
    fclose(fileOut);
    fclose(file);
}

void SkipHeader(FILE* file, FILE* fileOut)
{
    const int SIZE = 1;
    unsigned char data[SIZE];
    for (int i = 0; i < 16; i++)
    {
        fread(data, 1, SIZE, file);
        fwrite(data, 1, 1, fileOut);
    }
}
void IHDRChunk(FILE* file, FILE* fileOut, int width, int height, int bpp, int colortype)
{
    const int SIZE = 1;
    unsigned int data[SIZE];
    Decimaltohex(file, fileOut, width);
    Decimaltohex(file, fileOut, height);
    data[0] = bpp; //bpp
    fwrite(data, 1, 1, fileOut);
    fread(data, 1, SIZE, file);
    data[0] = colortype; //color
    fwrite(data, 1, 1, fileOut);
    fread(data, 1, SIZE, file);
    data[0] = 0; //compression
    fwrite(data, 1, 1, fileOut);
    fread(data, 1, SIZE, file);
    data[0] = 0; //filtertype
    fwrite(data, 1, 1, fileOut);
    fread(data, 1, SIZE, file);
    data[0] = 0; //interlace
    fwrite(data, 1, 1, fileOut);
    fread(data, 1, SIZE, file);

}
void Decimaltohex(FILE* file, FILE* fileOut, int value)
{
    const int SIZE = 1;
    unsigned int data[SIZE];
    for (int i = 3; i >= 0; i--)
    {
        int tvalue = Square(256, i);
        int a = value / tvalue;
        data[0] = value / tvalue;
        fwrite(data, 1, 1, fileOut);
        fread(data, 1, SIZE, file);
        value = value % tvalue;
    }
}

int Square(int a, int b)
{
    if (b == 0)
        return 1;
    else
        return a * Square(a, b - 1);
}
void FindIDATChunkAndCopyPaste(FILE* file, FILE* fileOut)
{
    const int SIZE = 2;
    unsigned char data[SIZE];
    for (int i = 0; i <= 50; i++)
    {
        fread(data, 1, SIZE, file);
        if ((int)data[0] == 73 && (int)data[1] == 68)  // ID 확인 
        {
            fwrite(data, 1, SIZE, fileOut);
            fread(data, 1, SIZE, file);
            if ((int)data[0] == 65 && (int)data[1] == 84) // AT 확인
            {
                fwrite(data, 1, SIZE, fileOut);
                return;
            }
            fwrite(data, 1, SIZE, fileOut);
        }
        if ((int)data[1] == 73) //I확인
        {
            fwrite(data, 1, SIZE, fileOut);
            fread(data, 1, SIZE, file);
            if ((int)data[0] == 68 && (int)data[1] == 65) //DA 확인
            {
                fwrite(data, 1, SIZE, fileOut);
                fread(data, 1, 1, file);
                if ((int)data[0] == 84) //T 확인
                {
                    fwrite(data, 1, 1, fileOut);
                    return;
                }
            }
            fwrite(data, 1, SIZE, fileOut);
        }
        fwrite(data, 1, SIZE, fileOut);
    }
    printf("IDAT chunk를 찾을 수 없습니다.");
    return; //IDAT영역 못찾음 ERROR 발생
}
void RGBtoBinary(FILE* file, FILE* fileOut, int LSIZE)
{
    unsigned char data[4];
    unsigned char binarydata[1];
    for (int i = 0; i < 153600; i = i + 4)
    {
        fread(data, 1, 4, file);
        R[i]   = (int)data[0];
        R[i+1] = (int)data[1];
        R[i+2] = (int)data[2];
        R[i+3] = (int)data[3];
    }
    for (int i = 0; i < 153600; i = i + 4)
    {
        fread(data, 1, 4, file);
        G[i] = (int)data[0];
        G[i + 1] = (int)data[1];
        G[i + 2] = (int)data[2];
        G[i + 3] = (int)data[3];
    }
    for (int i = 0; i < 153600; i = i + 4)
    {
        fread(data, 1, 4, file);
        B[i] = (int)data[0];
        B[i + 1] = (int)data[1];
        B[i + 2] = (int)data[2];
        B[i + 3] = (int)data[3];
    }
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        int Red = R[i], Green = G[i];
        int gray = (R[i] + B[i] + G[i]) / 3;
        if (gray > 127)             // bit 1
            binarydata[0] = 1;
        else                        // bit 0
            binarydata[0] = 0;
        fwrite(binarydata, 1, 1, fileOut);
    }
}
void IENDChunk(FILE* fileOut)
{
    const int SIZE = 4;
    unsigned char data[SIZE];
    data[0] = 0, data[1] = 0, data[2] = 0, data[3] = 0;
    fwrite(data, 1, 4, fileOut);
    data[0] = 73, data[1] = 69, data[2] = 78, data[3] = 68;
    fwrite(data, 1, 4, fileOut);
    data[0] = 174, data[1] = 66, data[2] = 96, data[3] = 130;
    fwrite(data, 1, 4, fileOut);
}
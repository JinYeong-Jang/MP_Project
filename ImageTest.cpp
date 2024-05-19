#include <stdio.h>
void SkipHeader(FILE* file, FILE* fileOut);
void IHDRChunk(FILE* file, FILE* fileOut, int width, int height, int bpp, int colortype);
void Decimaltohex(FILE* file, FILE* fileOut, int value);
int Square(int a, int b);
void FindIDATChunkAndCopyPaste(FILE* file, FILE* fileOut);
void RGBAtoBinary(FILE* file, FILE* fileOut, int LSIZE);
void IENDChunk(FILE* fileOut);


int main()
{
    const int LSIZE = 153600; // 960 x 160 pixels
    FILE* file = fopen("2024S_MA_image_rgba.png", "rb");
    FILE* fileOut = fopen("output_binary.png", "wb");
    SkipHeader(file, fileOut);
    IHDRChunk(file, fileOut, 960, 160, 1, 7);  //binary type은 존재하지 않기 때문에 colortype을 임의로 7로 설정
    FindIDATChunkAndCopyPaste(file, fileOut);
    RGBAtoBinary(file, fileOut, LSIZE);
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
void RGBAtoBinary(FILE* file, FILE* fileOut, int LSIZE)
{
    const int SIZE = 4;// RGBA 4byte 씩 읽어서 처리.
    unsigned char data[SIZE];
    for (int i = 0; i < LSIZE; i++) // RGB의 gray값이 중간을 넘으면 1로처리
    {                               // data[0]의 값을 write하기 때문에 data[0]를 0또는 1로 바꿔주면 된다.
        fread(data, 1, SIZE, file); // data[0] : R , data[1] : G , data[2] : B , data[3] : A
        int R = data[0], G = data[1], B = data[2], A = data[3];
        int Gray = (R + G + B) / 3;
        if (Gray > 127)             // bit 1
            data[0] = 1;
        else                        // bit 0
            data[0] = 0;
        fwrite(data, 1, 1, fileOut);
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
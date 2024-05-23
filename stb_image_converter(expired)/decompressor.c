#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#include "stb_image_write.h"
#define IMAGE_SIZE 960*640*4
#include "hex_to_array.h"

/*
int main()
{   
    char filename[] = "image.png";
    char outputimage[] = "outputimage.png";

    // ... x = width, y = height, n = # 8-bit components per pixel ...
    // ... replace '0' with '1'..'4' to force that many components per pixel
    // ... but 'n' will always be the number that it would have been if you said 0
    int x, y, n;
    unsigned char* data = stbi_load(filename, &x, &y, &n, 0);

    FILE *stream;
    stream = fopen("rgba_output.hex", "w+b");
    fwrite(data, sizeof(uint8_t), IMAGE_SIZE, stream);

    for (size_t i = 0; i < IMAGE_SIZE; i+=4) {
        data[i] = 255;
    }
    
    stbi_write_png("outputimage.png", 960, 640, 4, data, 960 * 4);

    //for (size_t i = 0; i < 1920; i++)
    //{
    //    printf("%d ", data[i]);
    //}
  
    stbi_image_free(data);
}*/


int main()
{
    char filename[] = "rgb.hex";
    char outputimage[] = "outputimage.png";

    // ... x = width, y = height, n = # 8-bit components per pixel ...
    // ... replace '0' with '1'..'4' to force that many components per pixel
    // ... but 'n' will always be the number that it would have been if you said 0
    
    //int x, y, n;
    //uint8_t data[IMAGE_SIZE/4];
    //unsigned char rgbcomp[IMAGE_SIZE/4];

    FILE* stream;
    stream = fopen("rgb.hex", "rb");
    char str[50000];
    fread(str, 50, 500, stream);
    printf("%s", str);
    //stbi_write_png(outputimage, 960, 640, 3, data, 960 * 3);



    //stbi_image_free(outputimage);
}
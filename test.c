#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void){
    char *a;
    for(int i=0; i<8; i++){
        a[i] = 255-i;
    }
    char *k;
    unsigned int mask = 0xE0E0E0E0;
    unsigned int tmp;
    tmp = (int)a[0] << 6 + (int)a[1] << 4 + (int)a[2] << 2 + (int)a[3];
    printf("%x", tmp);

    return 0;
}
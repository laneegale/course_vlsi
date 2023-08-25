#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *toBin(unsigned int num, int length) {
    char *binaryString = (char *)malloc(length + 1);  // +1 for the null terminator
    if (!binaryString) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = length - 1; i >= 0; i--) {
        binaryString[length - 1 - i] = ((num >> i) & 1) + '0';
    }
    
    binaryString[length] = '\0';
    
    return binaryString;
}

unsigned int operation(unsigned int a, unsigned int b) {
    unsigned int s = (a * b) % 13;
    return s & 0x0000000f;
}

int main() {

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            unsigned answer = operation(i, j);
            char* bin_re = toBin(answer, 4);
            printf("%s%s %s\n", toBin(i, 3), toBin(j, 3), bin_re);

        }
    }

    return 0;
}

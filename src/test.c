#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t data[] = {0x52b00006, 0xf2d00006, 0x2a860c21, 0x8a000000};
    FILE* fp = fopen("test.bin", "wb");
    if (fp == NULL) {
        printf("Failed to create the file.\n");
        return 1;
    }

    for (int i = 0; i < sizeof(data) / sizeof(uint32_t); i++) {
        fwrite(&data[i], sizeof(uint32_t), 1, fp);
    }

    fclose(fp);

    return 0;
}


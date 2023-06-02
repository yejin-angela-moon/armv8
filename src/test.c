#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t data[] = {0x58000040, 0x14000002, 0x000ace0f, 0x8a000000};
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


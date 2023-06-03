#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t data[] = {0xd2800160, 0xf9400002, 0xd2800061, 0xd1001421, 0xd280ccc3,0x8a000000 };
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

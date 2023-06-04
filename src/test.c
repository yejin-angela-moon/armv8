#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t data[] = {0xd2800042, 0xd2800020, 0xd2800c61, 
    0xf9009001, 0xd2802420,
    0xd1000442, 0xf100005f,
    0xf8001c01,  0x54ffffa1 , 0x8a000000};
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

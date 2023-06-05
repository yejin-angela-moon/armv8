#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t data[] = {0xd294d03c, 0xf2a0017c, 0x52800000, 0x72b14000, 0xb8000780, 0xd61f0380, 0x8a000000, 0x0000008a};
    FILE* fp = fopen("test.bin", "wb");
    if (fp == NULL) {
        printf("Failed to create the file. \n");
        return 1;
    }
    for (int i = 0; i < sizeof(data) / sizeof(uint32_t); i++) {
        fwrite(&data[i], sizeof(uint32_t), 1, fp);
       // print(&data[i]);
    }
    //fwrite(state, sizeof(uint32_t), 1, fp);
    fclose(fp);
    return 0;
}
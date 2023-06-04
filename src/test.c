#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t data[] = {0xd105f5ad, 0x512a7def, 0x6b0f79b7, 0x8a000000};
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
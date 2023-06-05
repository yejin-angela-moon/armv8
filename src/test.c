#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t data[] = {0xd28000a2, 0xd1000442, 0xd2800141, 0xd1000421, 0xeb02003f, 0x54ffffc1, 0xf100005f, 0x54ffff41, 0x8a000000};
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
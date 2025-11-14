#include <stdio.h>
#include <stdint.h>
#include <string.h>

static void write_u32_le(FILE *f, uint32_t v) {
    uint8_t b[4] = { v & 0xFF, (v>>8)&0xFF, (v>>16)&0xFF, (v>>24)&0xFF };
    fwrite(b,1,4,f);
}

int main(void){
    FILE *f = fopen("program.bin","wb");
    if(!f){ perror("fopen"); return 1; }

    const char *s1 = "123456789012345678901234567890123456789012345678901234567890";
    fputc(0x01, f); // PRINT
    write_u32_le(f, (uint32_t)strlen(s1));
    fwrite(s1,1,strlen(s1),f);

    const char *s2 = "Second message.";
    fputc(0x01, f); // PRINT
    write_u32_le(f, (uint32_t)strlen(s2));
    fwrite(s2,1,strlen(s2),f);

    fputc(0xFF, f); // HALT
    fclose(f);
    printf("Wrote program.bin\n");
    return 0;
}
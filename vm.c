// mini_vm_simple.c
// Tiny micro-VM: supports PRINT (0x01), NOP (0x02) and HALT (0xFF).W
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

FILE *decrypt_to_temp(const char *encpath, const char *key){
    FILE *fe = fopen(encpath,"rb");
    FILE *fd = tmpfile();
    if(!fe){ perror("fopen enc"); return NULL; }

    size_t klen = strlen(key);
    int c; size_t i = 0;
    while((c = fgetc(fe)) != EOF){
        unsigned char x = (unsigned char)c ^ (unsigned char)key[i % klen];
        if(fwrite(&x, 1,1,fd) != 1){
            printf("\nERROR");
            perror("write");
            fclose(fd); fclose(fe);
            return NULL;
        }
        i++;
    }
    fclose(fe);
    
    rewind(fd);
    return fd;
}
static uint32_t read_u32_le(const uint8_t *buf, size_t off, size_t sz) {
    if (off + 4 > sz) return 0;
    return (uint32_t)buf[off] | ((uint32_t)buf[off+1] << 8) |((uint32_t)buf[off+2] << 16) | ((uint32_t)buf[off+3] << 24);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s program.bin <Decrypt_KEY>\n", argv[0]);
        return 1;
    }

    const char *key = argv[2];
    const char *path = argv[1];
    FILE *f = decrypt_to_temp(path, key);
    if (!f) { perror("fopen"); return 2; }
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n <= 0) { fprintf(stderr, "empty program\n"); fclose(f); return 3; }
    uint8_t *buf = (uint8_t*)malloc(n);
    if (!buf) { perror("malloc"); fclose(f); return 4; }
    if (fread(buf, 1, n, f) != (size_t)n) { perror("fread"); free(buf); fclose(f); return 5; }
    fclose(f);

    size_t ip = 0;
    while (ip < (size_t)n) {
        uint8_t op = buf[ip++];
        switch (op) {
            case 0x01: { // PRINT
                if (ip + 4 > (size_t)n) { fprintf(stderr,"truncated PRINT length\n"); goto err; }
                uint32_t len = read_u32_le(buf, ip, n);
                ip += 4;
                if (ip + len > (size_t)n) { fprintf(stderr,"truncated PRINT data\n"); goto err; }
                // print string (not null-terminated)
                fwrite(&buf[ip], 1, len, stdout);
                putchar('\n');
                ip += len;
                break;
            }
            case 0x02: // NOP
                break;
            case 0xFF: // HALT
                goto done;
            default:
                fprintf(stderr, "unknown opcode 0x%02x at %zu\n", op, ip-1);
                goto err;
        }
    }
done:
    free(buf);
    return 0;
err:
    free(buf);
    return 6;
}

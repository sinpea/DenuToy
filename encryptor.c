// encryptor.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    if(argc != 4){
        fprintf(stderr, "Usage: %s <infile> <outfile> <key>\n", argv[0]);
        return 1;
    }
    const char *in = argv[1], *out = argv[2];
    const char *key = argv[3];
    size_t klen = strlen(key);
    FILE *fi = fopen(in, "rb");
    FILE *fo = fopen(out, "wb");
    if(!fi || !fo){ perror("fopen"); return 1; }

    int c; size_t i = 0;
    while((c = fgetc(fi)) != EOF){
        unsigned char x = (unsigned char)c ^ (unsigned char)key[i % klen];
        fputc(x, fo);
        i++;
    }
    fclose(fi); fclose(fo);
    printf("Encrypted %s -> %s\n", in, out);
    return 0;
}

#include <stdint.h>
#include <stdio.h>

int main (void) {
    FILE *a, *b, *c;
    a = fopen("keyfile", "r");
    b = fopen("flag.encrypted", "r");
    c = fopen("flag.clear", "w");
    char enc, key, clear;

    while (fread(&enc, 1, 1, b)) {
        fread(&key, 1, 1, a);
        clear = enc^key;
        fwrite(&clear, 1, 1, c);
    }
    fclose(a);
    fclose(b);
    fclose(c);
    return 0;
}

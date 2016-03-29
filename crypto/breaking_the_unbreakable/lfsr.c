#include <stdint.h>
#include <stdio.h>

int main (void) {
    uint8_t start_state = 0x29u;
    uint8_t lfsr = start_state;
    uint8_t bit;
    unsigned period = 0;

    FILE *file = fopen("keyfile", "w");

/* x^8 + x^7 + x^5 + x^3 + 1 */
    do {
        if((period % 8) == 0) fwrite(&lfsr, 1, 1, file);
        bit = ((lfsr >> 0 ) ^ (lfsr >> 1) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
        lfsr = (lfsr >> 1) | (bit << 7);
        ++period;
    } while (lfsr != start_state);
    fclose(file);
    return 0;
}

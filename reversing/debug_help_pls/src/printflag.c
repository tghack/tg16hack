#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* flags[] = {
    "TG16{str}",
    "TG16{cmp}",
    "TG16{Simula}",
    "TG16{UiO ftw}",
    "TG16{can this}",
    "TG16{flag here}",
    "TG16{mindgames}",
    "TG16{#tghacker}",
    "TG16{@quakenet}",
    "TG16{UiO-standen}",
    "TG16{flag_iz_her?}",
    "TG16{creativia has it}",
    "TG16{the flag is there}",
    "TG16{Petsovernight.com}",
    "TG16{pogothemonkey.com}",
    "TG16{Ole Johan Dahl ftw}",
    "TG16{this_the_real_flag}",
    "TG16{Flag for sale, cheap}",
    "TG16{Why are we even here}",
    "TG16{dance with me,David?}",
    "TG16{maybe_it's_this_one?}",
    "TG16{this is getting boring}",
    "TG16{Fernandos new beginnings}",
    "TG16{maybe it's the  oneabove}",
    "TG16{maybe it's the one below}",
    "TG16{maybe it's not even here}",
    "TG16{can't make any more soon}",
    "TG16{nah this is the real deal}",
    "TG16{filling_the_RAM_with_shit}",
    "TG16{I'm blue dabadeee daba da}",
    "TG16{this spot is for sale, you c}",
    "TG16{an advertise your stuff here}",
    "TG16{,contact the hacking crew if}",
    "TG16{you are interrested. GLHFGG!}"};

int main(void) {

    char flag[128] = { 0 };

    for (int i = 0; i < 34; i++) {
        flag[i] = flags[i][i];
    }

    printf("flag: %x\n", flag);
    return 0;
}

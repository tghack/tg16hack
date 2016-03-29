#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage(char* argv[]) {
    printf("USAGE: %s <password>\n", argv[0]);
    exit(1);
}

int main(int argc, char* argv[]) {
    if(argc < 2) usage(argv);
    char* pwd = argv[1]; 
    if(strlen(pwd) != 4) exit(1);
    if(pwd[3] != 'd') exit(1);
    if(pwd[2] != '1') exit(1);
    if(pwd[1] != 'A') exit(1);
    if(pwd[0] != 'p') exit(1);

    printf("gz! The flag is TG16{<password>}\n");
}

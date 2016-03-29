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
    if(strlen(pwd) != 1) {
        printf("The password is only 1 char\n");
        exit(1);
    }
    if(pwd[0] != '\"') exit(1);
    /* this is to make it a bit less trivial to randomly hit it
     * while running the program, as the shell would need
     * the " to be escaped.
     */

    printf("gz! The flag is TG16{<password>}\n");
}

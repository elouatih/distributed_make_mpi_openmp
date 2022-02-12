#include <stdio.h>
#include "parser.c"

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("%sNeed 1 arguments \n", KRED);
        exit(EXIT_FAILURE);
    }
    char *src = argv[1];
    char *dest = "Makefile";
    char *filename;
    char *path;
    char *cmd;
    char *src1 = "cd ";
    size_t fullSize, cmdSize;
    fullSize = strlen( src ) + 1 +  strlen( dest ) + 1;
    cmdSize = strlen( src1 ) + 1 +  strlen( src ) + 1;
    size_t pathSize = strlen(src) + 1 + 2;
    char *exists = (argv[1][strlen(argv[1]) - 1] == '/') ? "" : "/";
    filename = (char *) malloc( fullSize );
    path = (char *) malloc(pathSize);
    cmd = (char *) malloc(cmdSize);
    strcpy(filename, src);
    strcpy(cmd, src1);
    strcat(filename, exists);
    strcpy(path, filename);
    //printf("%s \n", path);
    strcat(cmd, src);
    strcat(filename, dest);

    make_distribue(argc, argv, filename, path, cmd);
    //make_simple(filename, cmd);
    /*add_machine_working("localhost");
    add_machine_working("localhost");
    add_machine_working("dahu-2.grenoble.access.grid5000.fr");
    add_machine_working("dahu-2.grenoble.access.grid5000.fr");
    add_machine_working("dahu-6.grenoble.access.grid5000.fr");
    add_machine_working("dahu-6.grenoble.access.grid5000.fr");
    add_machine_working("dahu-4.grenoble.access.grid5000.fr");
    //send_machine_working("localhost", path);*/
    return 0;
}

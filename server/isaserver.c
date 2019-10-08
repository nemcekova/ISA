/* Project: Programovanie sietovej sluzby
*  variant: HTTP nastenka
*  author: Barbora Nemcekova
*  login: xnemce06
*  date: 08.10.2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]){
    char *portnum;
    if(argc > 3){
        printf("Too many arguments\n");
        return 0;
    }
    int opt;
    while ((opt = getopt(argc, argv, ":hp")) != -1) {
        switch (opt) {
            case 'h':
                printf("HELP\n");
                break;
            case 'p':
                printf("PORT\n");
                continue;
            default:
                printf("Invalid argument\n");
                exit(-1);
        }
    }
    
    if(argc == 3 && (strcmp(argv[1], "-p") == 0)){
        //for(; optind < argc; optind++){      
            printf("extra arguments: %s\n", argv[optind]);
            portnum = argv[optind];
            printf("%s\n", portnum);  
        //} 
    }
    return 0;
}
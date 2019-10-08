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
    
    char* host;
    char* port;
    
    int opt;
    while ((opt = getopt(argc, argv, ":hpH")) != -1) {
        
        switch (opt) {
            case 'h':
                printf("HELP\n");
                break;
            case 'p':
                printf("PORT\n");
                continue;
            case 'H':
                printf("HOST\n");
                continue;
            default:
                printf("Invalid argument\n");
                exit(-1);
        }
    }
    
    return 0;
}
/* Project: Programovanie sietovej sluzby
*  variant: HTTP nastenka
*  author: Barbora Nemcekova
*  login: xnemce06
*  date: 08.10.2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>

#define BUFFER 1024

int main(int argc, char *argv[]){
    
    char* host;
    char* port;
    char* command;
    
    
    if(strcmp(argv[1], "-h") == 0){
        printf("HELP\n");
        exit(0);
    }
    
    else if((strcmp(argv[1], "-H") == 0) && (strcmp(argv[3], "-p") == 0)){
        host = argv[2];
        port = argv[4];
        
        printf("%s\n", host);
        printf("%s\n", port);
    }
    
    else if((strcmp(argv[1], "-p") == 0) && (strcmp(argv[3], "-H") == 0)){
        port = argv[2];
        host = argv[4];
        
        printf("%s\n", host);
        printf("%s\n", port);
        
    }
    
    else{
        fprintf(stderr, "Invalid arguments\n");
    }
    
    if ((strcmp(argv[5], "boards") == 0) && argc == 6){
        //GET / boards
    }
    else if((strcmp(argv[5], "board") == 0) && argc == 8){
        //board add/delete/list
    }
    else if((strcmp(argv[5], "item") == 0) && argc == 9){
        //item add delete update
    }
    else{
        fprintf(stderr, "Invalid arguments\n");
    }
    
    int sock;
    socklen_t leng;
    struct sockaddr_in client;
    struct sockaddr_in server;
    struct hostent *servaddr;
    char buffer[BUFFER];
    
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));
    
    //check host name / address
    server.sin_family = AF_INET;
    if((servaddr = gethostbyname(host)) == NULL){
        errx(1, "Couldnt get host\n");
    }
    printf("host ok\n");
    
    memcpy(&server.sin_addr, servaddr->h_addr, servaddr->h_length);
    server.sin_port = htons(atoi(port));
    
    //create socket
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        err(1, "socket was not created\n");
    }
    printf("socket was created\n");
    
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1){
        err(1, "could not connect to server");
    }
    
    leng = sizeof(client);
    if (getsockname(sock, (struct sockaddr*)&client, &leng) == -1) {
        err(1, "could not get socket");
    }
    
    printf("Client succesfully connected\n");
    
    strcpy(buffer, "GET /board/name HTTP/1.1\nContent-Type: text/plain\nContent-Length: 5\n\ntest\n");
    int j = write(sock, buffer, 74);
    if(j == -1){
        err(1, "write failed");
    }
    
    return 0;
}
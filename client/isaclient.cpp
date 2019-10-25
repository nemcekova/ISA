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
#include<ctype.h>
#include<vector>
#include<map>
#include<string>
#include<iostream>
#include<algorithm>
#include<iterator>
#include<regex>
#include<sstream>
#define BUFFER 1024

int main(int argc, char *argv[]){
    
    char* host;
    char* port;
    char* command;
    std::string method;
    std::string where;
    std::string name;
    std::string id;
    std::string content;
    std::string message;
    std::stringstream ss;
    
    std::string argumenty;
    for(int i=1; i<argc; i++){
        argumenty.append(argv[i]);
        argumenty.append(" ");
    }
    //std::cout << argumenty;
    
    if(strcmp(argv[1], "-h") == 0){
        printf("HELP\n");
        exit(0);
    }
    
    else if((strcmp(argv[1], "-H") == 0) && (strcmp(argv[3], "-p") == 0)){
        host = argv[2];
        port = argv[4];
    }
    
    else if((strcmp(argv[1], "-p") == 0) && (strcmp(argv[3], "-H") == 0)){
        port = argv[2];
        host = argv[4];    
    }
    
    else{
        fprintf(stderr, "Invalid arguments\n");
    }
    
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    
    if ((strcmp(argv[5], "boards") == 0) && argc == 6){
        //GET / boards
                
        message = "GET /boards HTTP/1.1\n\n";
        
    }
    else if((strcmp(argv[5], "board") == 0) && argc == 8){
        //board add/delete/list
        if (strcmp(argv[6], "add") == 0){
            name = argv[7];
            
            ss << "POST /boards/" << name << " HTTP/1.1\n\n";
            message = ss.str(); 
        }
        else if(strcmp(argv[6], "delete") == 0){
            name = argv[7];
            
            ss << "DELETE /boards/" << name << " HTTP/1.1\n\n";
            message = ss.str();
        }
        else if(strcmp(argv[6], "list")==0){
            method = "GET";
            where = "board";
            name = argv[7];
            
            ss << "GET /board/" << name << " HTTP/1.1\n\n";
            message = ss.str();
            
        }
    }
    else if((strcmp(argv[5], "item") == 0) && (strcmp(argv[6], "update") == 0)){
        name = argv[7];
        id = argv[8];
        std::smatch m;
        std::regex r("item\\supdate\\s([a-zA-Z0-9]+)\\s([0-9]+)\\s(.+)$");
        if(regex_search(argumenty, m, r) == true){
            content = m[3];
            //std::cout << content << "\n";
            int len = content.length();
            ss << "PUT /board/" << name << "/" << id << " HTTP/1.1\nContent-Type: text/plain\nContent-Length: " << len << "\n\n" << content;
            message = ss.str();
        }
        else{
            std::cout << "Invalid args\n";
        }
    }
    else if(strcmp(argv[5], "item") == 0){
        //item add delete
        if(strcmp(argv[6], "add") == 0){
            method = "POST";
            where = "board";
            name = argv[7];
            std::smatch m;
            std::regex r("item\\sadd\\s([a-zA-Z0-9]+)\\s(.+)$");
            if(regex_search(argumenty, m, r) == true){
                content = m[2];
                std::cout << content << "\n";
                int len = content.length();
                ss << "POST /board/" << name << " HTTP/1.1\nContent-Type: text/plain\nContent-Length: " << len << "\n\n" << content;
                message = ss.str();
            }
        }
        
        else if(strcmp(argv[6], "delete") == 0){
            method = "DELETE";
            where = "board";
            name = argv[7];
            id = argv[8];
            
            ss << "DELETE /board/" << name << "/" << id << " HTTP/1.1\n\n";
            message = ss.str();
        }
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
    //printf("host ok\n");
    
    memcpy(&server.sin_addr, servaddr->h_addr, servaddr->h_length);
    server.sin_port = htons(atoi(port));
    
    //create socket
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        err(1, "socket was not created\n");
    }
    //printf("socket was created\n");
    
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1){
        err(1, "could not connect to server");
    }
    
    leng = sizeof(client);
    if (getsockname(sock, (struct sockaddr*)&client, &leng) == -1) {
        err(1, "could not get socket");
    }
    
    //printf("Client succesfully connected\n");
    
    int l = message.length();
    char message_array[l +1];
    strcpy(message_array, message.c_str());
    
    strcpy(buffer, message_array);
    int j = write(sock, buffer, 100);

    if(j == -1){
        err(1, "write failed");
    }
    
    if ((j = read(sock,buffer, BUFFER)) == -1)   // read the answer from the server
     err(1,"read() failed");
   else if (j > 0){
  
       std::string buf(buffer);
       std::string delimiter = "\n\n";
       std::string token;
       size_t pos = 0;
       int k = 1;
       
       //toto rozdeli odpoved podla \n\n ale treba to dako identifikovat na header a body
       while ((pos = buf.find(delimiter)) != std::string::npos) {
           std::cout << k << "\n";
           token = buf.substr(0, pos);
            std::cout << token << std::endl;
            buf.erase(0, pos + delimiter.length());
            k++;
        }
        std::cout << k << "\n";
        std::cout << buf << std::endl;
       /*std::smatch m;
       std::regex res_reg("^(.+)\\n(.+)\\n(.+)\\n\\n(.+)$");
       if(std::regex_search(buf, m, res_reg) == true){
           std::string header = m[1] + m[2] + m[3];
           std::string text = m[4];
           std::cout << header << "\n";
           std::cout << text << "\n";
       }*/
    //char *header = strtok(buffer, "\n\n");
    //std::cout << header;
}
   
     //printf("%.*s",j,text);                   // print the answer
 
    
    return 0;
}
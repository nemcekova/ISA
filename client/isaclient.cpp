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
#define BUFFER 4096


int main(int argc, char *argv[]){
    
    int ret;
    char* host;
    char* port;
    std::string method;
    std::string where;
    std::string name;
    std::string id;
    std::string content;
    std::string message;
    std::stringstream ss;
    
    
    //kontrola argumentov
    std::string argumenty;
    for(int i=1; i<argc; i++){
        argumenty.append(argv[i]);
        argumenty.append(" ");
    }
    
    //vypíše help a skončí
    if(strcmp(argv[1], "-h") == 0){
        printf("Vitajte v programe isaclient.cpp pre projekt do predmetu ISA\nProgram spustíte s nasledujúcimi argumentami:\n-H <host> -p <port> <command>\n\n<command> môže mať nasledujúce formy:\nboards - zobrazí názvy násteniek\nboard add <name> - vytvorí nástenku s názvon <name>\nboard delete <name> - vymaže celú nástenku s názvom <name>\nboard list <name> - zobrzí obsah nástenky s názvom <name>\nitem add <name> <content> - na koniec nástenky s názvom <name> pridá príspevok s obsahom <content>\nitem delete <name> <id> - na nástenke name odstráni príspevok s číslom <id>\nitem update <name> <id> <content> - na nástenke s názvom <name> zmení obsah príspevku číslo <id> na obsah <content>\n");
        return(0);
    }
    
    else if(argc < 6){
        std::cerr << "Invalid arguments\n";
        printf("Vitajte v programe isaclient.cpp pre projekt do predmetu ISA\nProgram spustíte s nasledujúcimi argumentami:\n-H <host> -p <port> <command>\n\n<command> môže mať nasledujúce formy:\nboards - zobrazí názvy násteniek\nboard add <name> - vytvorí nástenku s názvon <name>\nboard delete <name> - vymaže celú nástenku s názvom <name>\nboard list <name> - zobrzí obsah nástenky s názvom <name>\nitem add <name> <content> - na koniec nástenky s názvom <name> pridá príspevok s obsahom <content>\nitem delete <name> <id> - na nástenke name odstráni príspevok s číslom <id>\nitem update <name> <id> <content> - na nástenke s názvom <name> zmení obsah príspevku číslo <id> na obsah <content>\n");
        
        return(-1);
    }
    //-H nastaví host, -p nastaví číslo portu
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
        printf("Vitajte v programe isaclient.cpp pre projekt do predmetu ISA\nProgram spustíte s nasledujúcimi argumentami:\n-H <host> -p <port> <command>\n\n<command> môže mať nasledujúce formy:\nboards - zobrazí názvy násteniek\nboard add <name> - vytvorí nástenku s názvon <name>\nboard delete <name> - vymaže celú nástenku s názvom <name>\nboard list <name> - zobrzí obsah nástenky s názvom <name>\nitem add <name> <content> - na koniec nástenky s názvom <name> pridá príspevok s obsahom <content>\nitem delete <name> <id> - na nástenke name odstráni príspevok s číslom <id>\nitem update <name> <id> <content> - na nástenke s názvom <name> zmení obsah príspevku číslo <id> na obsah <content>\n");
        
        return(-1);
    }
    
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    
    /*
    *Zostavenie správy na základe argumentov od užívateľa
    */
    //boards
    if ((strcmp(argv[5], "boards") == 0) && argc == 6){                
        message = "GET /boards HTTP/1.1\n\n";
    }
    
    else if((strcmp(argv[5], "board") == 0) && argc == 8){
        //board add <name>
        if (strcmp(argv[6], "add") == 0){
            name = argv[7];
            ss << "POST /boards/" << name << " HTTP/1.1\r\n\r\n";
            message = ss.str(); 
        }
        //board delete <name>
        else if(strcmp(argv[6], "delete") == 0){
            name = argv[7];
            ss << "DELETE /boards/" << name << " HTTP/1.1\r\n\r\n";
            message = ss.str();
        }
        //board list <name>
        else if(strcmp(argv[6], "list")==0){
            method = "GET";
            where = "board";
            name = argv[7];
            ss << "GET /board/" << name << " HTTP/1.1\r\n\r\n";
            message = ss.str();
        }
    }
    //item update <name> <id> <content>
    else if((strcmp(argv[5], "item") == 0) && (strcmp(argv[6], "update") == 0)){
        name = argv[7];
        id = argv[8];
        std::smatch m;
        std::regex r("item\\supdate\\s([a-zA-Z0-9]+)\\s([0-9]+)\\s(.+)$");
        if(regex_search(argumenty, m, r) == true){
            content = m[3];
            int len = content.length();
            ss << "PUT /board/" << name << "/" << id << " HTTP/1.1\r\nContent-Type: text/plain\r\nContent-Length: " << len << "\r\n\r\n" << content;
            message = ss.str();
        }
        else{
            std::cout << "Invalid args\n";
            return(-1);
        }
    }
    
    else if(strcmp(argv[5], "item") == 0){
        //item add <name> <content>
        if(strcmp(argv[6], "add") == 0){
            method = "POST";
            where = "board";
            name = argv[7];
            std::smatch m;
            std::regex r("item\\sadd\\s([a-zA-Z0-9]+)\\s(.+)$");
            if(regex_search(argumenty, m, r) == true){
                content = m[2];
                int len = content.length();
                ss << "POST /board/" << name << " HTTP/1.1\r\nContent-Type: text/plain\r\nContent-Length: " << len << "\r\n\r\n" << content;
                message = ss.str();
            }
        }
        
        //item delete <name> <id>
        else if(strcmp(argv[6], "delete") == 0){
            method = "DELETE";
            where = "board";
            name = argv[7];
            id = argv[8];
            ss << "DELETE /board/" << name << "/" << id << " HTTP/1.1\r\n\r\n";
            message = ss.str();
        }
    }
    else{
        std::cerr << "Invalid arguments\n";
        return(-1);
    }
    
    int sock;
    socklen_t leng;
    struct sockaddr_in client;
    struct sockaddr_in server;
    struct hostent *servaddr;
    char buffer1[BUFFER];
    char buffer2[BUFFER];
    
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));
    
    //kontrola hostname alebo ip adresy
    server.sin_family = AF_INET;
    if((servaddr = gethostbyname(host)) == NULL){
        errx(1, "Couldnt get host\n");
    }
        
    memcpy(&server.sin_addr, servaddr->h_addr, servaddr->h_length);
    server.sin_port = htons(atoi(port));
    
    //vytvorí socket
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        err(1, "socket was not created\n");
    }
    //printf("socket was created\n");
    
    //pripojenie na server
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1){
        std::cerr << "Couldnt connect to server\n";
        exit(1);
        //err(1, "could not connect to server");
    }
    
    leng = sizeof(client);
    if (getsockname(sock, (struct sockaddr*)&client, &leng) == -1) {
        err(1, "could not get socket");
    }
    
    //printf("Client succesfully connected\n");
    
    //vyrátanie veľkosti správy
    int l = message.length();
    char* message_array = new char[l +1];
    strcpy(message_array, message.c_str());
    
    //odoslanie správy na server
    strcpy(buffer1, message_array);
    int j = write(sock, buffer1, 100);

    if(j == -1){
        err(1, "write failed");
    }
    
    //načítanie odpovede zo serveru
    if ((j = read(sock,buffer2, BUFFER)) == -1){   // read the answer from the server
        err(1,"read() failed");
    }
   else if (j > 0){
       std::string buf(buffer2);
       std::string delimiter = "\r\n\r\n";
       std::string token;
       std::string header;
       std::string text;

        
        std::size_t position = buf.find("\r\n\r\n");
        header = buf.substr(0, position);
        text = buf.substr(position + 4, std::string::npos);
        
        //zistenie úspešnosti akcie na serveri
        std::regex r("^HTTP/1.1 (.+)\r\n");
        std::smatch m;
        std::string code;
        if (std::regex_search(header, m, r)) {
            code = m[1];
            
            //akcia bola úspešná, isaclient vráti 0
            if(code == "200 Ok" || code == "201 Created"){
                ret = 0;
            }
            //akcia na serveri bola neúspešná, isaclient vráti -1
            else{
                ret = -1;
            }
        }
        else{
            ret = -1;
        }
        //vypísanie odpovede
        //hlavičku na chybový výstup
        //telo správy na štandardný výstup v prípade ze je dĺžka správy väčšia ako 0
        std::cerr << header << "\n";
        
        transform(header.begin(), header.end(), header.begin(), ::tolower);
                
        std::regex r_content("content-lenght:(.+)");
        std::smatch m2;
        int value;
        std::string s;
        if(std::regex_search(header, m2, r_content)){
            s = m2[1];
            value = atoi(s.c_str());
            if (value > 0) {
                std::cout << text;
            }
        }
    }
    close(sock);
    return(ret);
}
/* Project: Programovanie sietovej sluzby
*  variant: HTTP nastenka
*  author: Barbora Nemcekova
*  login: xnemce06
*  date: 08.10.2019
*/

#include "isaserver.h"


#define BUFFER 1024
#define QUEUE 1



class Board{
    public:
        Board(){
            std::cout << "Hello World!";
        }
        ~Board();
        std::string name;
        
        //vyhladavanie ci nastenka s danym nazvom existuje
        bool BoardExists(std::string name){
            for (unsigned int i = 0; i < nastenky.size(); i++){
                if(name.compare(nastenky[0]) == 0){
                    return true;
                }
                else continue;
            }
            return false;
        }
        
        void AddNewBoard(std::string name){
            nastenky.push_back(name);
            printf("%s\n", name);
        }
        
    private:
        std::vector<std::string> nastenky; // zoznam aktualnych nasteniek
        std::map<std::string, int> content; //mapovanie obsahu na nastenku
        
        
    
};


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
            portnum = argv[optind];
    }
    
    
    
    
    //undefined reference to `Board::~Board()'
    Board nova;
    nova.AddNewBoard("prva");
    
    
    
    
    
    
    
    
    
    /*int fd;
    int sock;
    int leng;
    int size;
    int i;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buffer[BUFFER];
    
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        fprintf(stderr, "could not create socket\n" );
    }
    printf("socket created\n");
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(portnum));
    
    //bind socket to port
    if(bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0){
        err(1,"binding failed");
    }
    printf("Binding succesfull\n");
    
    if(listen(fd, QUEUE) != 0){
        err(1, "Listen() not possible");
    }
    printf("Listening...\n");
    
    leng = sizeof(client);
    while(1){
        if((sock = accept(fd, (struct sockaddr *)&client, (socklen_t *)&leng)) == -1){
            err(1, "accept failed");
        }
        printf("connection accepted\n");
    
        if((size = read(sock, buffer, BUFFER)) > 0){
            printf("A new connection accepted\n");
        }
        strcpy(buffer, "200 Ok, server is Listening");
        size = strlen(buffer);
        i = write(sock, buffer, size);
        if(i != size){
            err(1, "write() failed");
        }
    }
    */
    return 0;
}
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
    std::string name;
    public:
        
        Board(std::string boardname){
            std::cout << "Hello World!\n";
            name = boardname;
            //AddNewBoard(name);
            //nastenky.push_back(name);
        }
        ~Board(){
            std::cout << "Bye World!\n";
        }
        //std::string name;
        
        std::string GetName(){
            return(name);
        }
        
        void AddContent(std::string text){
            if(content.empty() == true){
                i = 1;
                content.insert(std::pair<int, std::string>(i, text));
                i++;
            }
            else{
                content.insert(std::pair<int, std::string>(i, text));
                i++;
            }
        }
        
        void DeleteContent(int id){
            bool changing = false;
            std::string key;
            std::map<int,std::string>::iterator it = content.begin();
            for (it=content.begin(); it!=content.end(); ++it){
                /*if(changing == true){
                    it->first = key; ///////////NEFUNGUJE TOTO JE READ ONLY
                    key++;
                }*/
                if(it->first == id){
                std::map<int, std::string>::iterator dupe = it;
                //std::map<int, std::string>::iterator dupe2 = dupe2;
                for(dupe = it; dupe != content.end();++dupe){
                    //for(dupe2 = dupe; dupe2!=content.end();++dupe2){
                        key = dupe->second;
                        it->second = key;
                    //}
                    //content.erase(it);
                    }
                }
            }
        }
        
        std::map<int, std::string> GetContent(){
            return(content);
        }
        
    private:
        std::map<int, std::string> content; //mapovanie obsahu na nastenku
        int i;
        //void AddNewBoard(std::string name){
            
        //}
};

/////////////////////////////////////////////////////////////////////////////////////
class List{
    std::string name;
    std::vector<std::string> nastenky; // zoznam aktualnych nasteniek
public:
    List(std::string listname){
        name = listname;
    }
    
    ~List(){
        name = "";
    }

    /*
    *Checks if board with specific name already exists
    */
    bool BoardExists(std::string BoardName){
        for (unsigned int i = 0; i < nastenky.size(); i++){
            if(BoardName.compare(nastenky[i]) == 0){
                std::cout << "true\n";
                return true;
            }
            else continue;
        }
        std::cout << "false\n";
        return false;
    }

    /*  
    * add new board to the list 
    */
    void AddNewBoard(std::string nazov){
        bool exists = BoardExists(nazov);
        if(exists == false){
            nastenky.push_back(nazov);
        }
    }
    
    /*
    *delete board from the list of boards
    */
    void DeleteBoard(std::string nazov){
        std::vector<std::string>::iterator it;
        
        it = find(nastenky.begin(), nastenky.end(), nazov);
        if(it != nastenky.end()){
            nastenky.erase(it);
        }
    }
    
    /*
    *returns list of boards
    */
    std::vector<std::string> GetNastenky(){
        return(nastenky);
    }
    
};

////////////////////////////////////////////////////////////////////////////////////////
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
    Board nova("prva");
    List zoznam("novy");
    zoznam.AddNewBoard(nova.GetName());
    Board nastenka("druha");
    zoznam.AddNewBoard(nastenka.GetName());
    
    Board bbb("tretia");
    zoznam.AddNewBoard(bbb.GetName());
    
    std::vector<std::string> nastenky = zoznam.GetNastenky();
    for(unsigned int i =0; i<nastenky.size(); i++){
        std::cout << nastenky[i] + "\n";
        
    }
    
    zoznam.DeleteBoard(nova.GetName());
    std::vector<std::string> naste = zoznam.GetNastenky();
    for(unsigned int o =0; o<naste.size(); o++){
        std::cout << naste[o] + "\n";
        
    }
    
    nova.AddContent("aaaaaaaaa\n");
    nova.AddContent("bbbbbbbbb\n");
    nova.AddContent("ccccccccc\n");
    nova.AddContent("ddddddddd\n");
    nova.AddContent("eeeeeeeee\n");
    nova.AddContent("fffffffff\n");
    nova.AddContent("ggggggggg\n");
    
    std::map<int, std::string> obsah = nova.GetContent();
    
    
    std::map<int,std::string>::iterator it = obsah.begin();
    for (it=obsah.begin(); it!=obsah.end(); ++it)
    std::cout << it->first << " " << it->second << '\n';
    //nova.AddNewBoard("prva");
    
    nova.DeleteContent(2);
    
    std::map<int, std::string> obsa = nova.GetContent();
     it = obsa.begin();
    for (it=obsa.begin(); it!=obsa.end(); ++it)
    std::cout << it->first << " " << it->second << '\n';
    
    
    
    
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
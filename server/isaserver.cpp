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
        
        void UpdateContent(int id, std::string new_value){
            std::map<int,std::string>::iterator it = content.begin();
            for (it=content.begin(); it!=content.end(); ++it){
                if(it->first == id){
                    it->second = new_value;
                }
            }
        }
        
        void DeleteContent(int id){
            std::string key;
            std::map<int,std::string>::iterator it = content.begin();
            for (it=content.begin(); it!=content.end(); ++it){
                
                if(it->first == id){
                std::map<int, std::string>::iterator dupe = it;
                for(dupe = it; dupe != content.end();++dupe){
                    key = dupe->second;
                    it->second = key;
                    it = dupe;
                }
                content.erase(it);
                break;
                }
            }
        }
        
        std::map<int, std::string> GetContent(){
            return(content);
        }
        
    private:
        std::map<int, std::string> content; //mapovanie obsahu na nastenku
        int i;

};

/////////////////////////////////////////////////////////////////////////////////////
class List{
    std::string name;
    std::vector<std::string> nastenky; // zoznam aktualnych nasteniek
    std::map<std::string, Board*> odkazy;
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
    void AddNewBoard(std::string nazov, Board* nastenka){
        /*if(std::regex_match(nazov, std::regex("[a-zA-Z0-9]"))){
            std::cout << "regex ok\n";
        }
        else{
            std::cout << "regex not ok\n";
            exit(1);
        }*/
        bool exists = BoardExists(nazov);
        if(exists == false){
            nastenky.push_back(nazov);
            odkazy.insert(std::pair<std::string, Board*>(nazov, nastenka));
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
            odkazy.erase(nazov);
        }
    }
    
    /*
    *returns list of boards
    */
    std::vector<std::string> GetNastenky(){
        return(nastenky);
    }
    
    Board* GetOdkaz(std::string key){
        bool exists = BoardExists(key);
        if( exists == true){
            std::map<std::string, Board*>::iterator it;
            it = odkazy.find(key);
            return(it->second);
        }
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
    
    
//////////////////////////////////////////////////////////////////////////////////    
    
/*    //undefined reference to `Board::~Board()'
    Board* nova = new Board("prva");
    //Board nova("prva");
    List zoznam("novy");
    zoznam.AddNewBoard(nova->GetName());
    Board nastenka("druha");
    zoznam.AddNewBoard(nastenka.GetName());
    
    Board bbb("tretia");
    zoznam.AddNewBoard(bbb.GetName());
    
    
    std::vector<std::string> nastenky = zoznam.GetNastenky();
    for(unsigned int i =0; i<nastenky.size(); i++){
        std::cout << nastenky[i] + "\n";
        
    }
    
    delete nova;
    std::vector<std::string> naste = zoznam.GetNastenky();
    for(unsigned int o =0; o<naste.size(); o++){
        std::cout << naste[o] + "\n";
        
    }
    
    nova->AddContent("aaaaaaaaa\n");
    nova->AddContent("bbbbbbbbb\n");
    nova->AddContent("ccccccccc\n");
    nova->AddContent("ddddddddd\n");
    nova->AddContent("eeeeeeeee\n");
    nova->AddContent("fffffffff\n");
    nova->AddContent("ggggggggg\n");
    
    std::map<int, std::string> obsah = nova->GetContent();
    
    
    std::map<int,std::string>::iterator it = obsah.begin();
    for (it=obsah.begin(); it!=obsah.end(); ++it)
    std::cout << it->first << " " << it->second << '\n';
    //nova.AddNewBoard("prva");
    zoznam.DeleteBoard(nova->GetName());
    delete nova;
    printf("------------------------------------\n");
    nova->DeleteContent(3);
    
    std::map<int, std::string> obsa = nova->GetContent();
     it = obsa.begin();
    for (it=obsa.begin(); it!=obsa.end(); ++it)
    std::cout << it->first << " " << it->second << '\n';*/
    
    
/////////////////////////////////////////////////////////////////////////////////
    
    
    int fd;
    int sock;
    int leng;
    int size;
    int i;
    std::string method;
    std::string where;
    std::string name;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buffer[BUFFER];
    List* zoznam = new List("novy");
    int ret;
    std::string text;
    int id;
    
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
            std::cout << buffer << "\n";
            printf("A new connection accepted\n");
        }
        //strcpy(buffer, "200 Ok, server is Listening");
        strcpy(buffer, "HTTP/1.1 200 Ok\nContent-Type: text/plain\nContent-Length: 5\n\ntest\n");
        std::cout << "Ide tam: \n" << buffer << "\n";
        size = strlen(buffer);
        i = write(sock, buffer, size);
        if(i != size){
            err(1, "write() failed");
        }
    }
    
    
    if(method == "POST" && where == "boards"){
        //ak existuje nastenka s danym nazvom, vracia 409
        bool exist = zoznam->BoardExists(name);
        if(exist == true){
                ret = 409;
        }
        //vytvori novu prazdnu nastenku typu Board a prida ju na zoznam nasteniek
        //kod uspecu je 201 pre metodu POST
        else{
            Board* new_board = new Board(name);
            zoznam->AddNewBoard(name, new_board);
            ret = 201;
        }
    }
    
    else if(method == "GET" && where == "boards"){
        std::vector<std::string> nastenky = zoznam->GetNastenky();
        //ak ziadna nastenka neexistuje, kod uspechu je 404
        if(nastenky.empty()){
            ret = 404;    
        }
        //ak zoznam nasteniek nie je prazdny, kod uspechu je 200 
        else{
            ret = 200;
        } 
    }
    
    else if(method == "DELETE" && where == "boards"){
        bool exist = zoznam->BoardExists(name);
        if(exist==false){
            ret = 404;
        }
        //odstrani nastenku zo zoznamu a nasledne aj samotnu nastenku
        else{
            Board* del = zoznam->GetOdkaz(name);
            zoznam->DeleteBoard(name);
            delete del;
        }
    }
    
    else if(method == "GET" && where == "board"){
        bool exist = zoznam->BoardExists(name);
        if(exist==false){
            ret = 404;
        }
        else{
            Board* show = zoznam->GetOdkaz(name);
            show->GetContent();
            ret = 200;
        }
    }
    
    else if(method == "POST" && where == "board"){
        if(text.size() == 0){
            ret = 400;
        }
        bool exist = zoznam->BoardExists(name);
        if(exist==false){
            ret = 404;
        }
        else{
            Board* board = zoznam->GetOdkaz(name);
            board->AddContent(text);
            ret = 201;
        }
    }
    
    else if(method == "PUT" && where == "board"){
        if(text.size() == 0){
            ret = 400;
        }
        bool exist = zoznam->BoardExists(name);
        if(exist==false){
            ret = 404;
        }
        else{
            Board* board = zoznam->GetOdkaz(name);
            board->UpdateContent(id, text);
            ret = 200;
        }
    }
    
    else if(method == "DELETE" && where == "board"){
        bool exist = zoznam->BoardExists(name);
        if(exist==false){
            ret = 404;
        }
        else{
            Board* board = zoznam->GetOdkaz(name);
            board->DeleteContent(id);
            ret = 200;
        }
    }
    
    return 0;
}
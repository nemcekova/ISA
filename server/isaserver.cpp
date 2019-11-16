/* Project: Programovanie sietovej sluzby
*  variant: HTTP nastenka
*  author: Barbora Nemcekova
*  login: xnemce06
*  date: 08.10.2019
*/

#include "isaserver.h"


#define BUFFER 4096
#define QUEUE 1


//////////////////////////////////////////////////////////////////////////////////
///////////////////////////// CLASS BOARD ////////////////////////////////////////

class Board{
    std::string name;
    public:
        //konštruktor objektu 
        Board(std::string boardname){
            name = boardname;
        }
        //deštruktor objektu
        ~Board(){    
        }
        
        //funkcia vráti názov nástenky
        std::string GetName(){
            return(name);
        }
        
        /*
        *metoda prida novy prispevok do nastenky na koniec
        */
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
        
        /*
        *funkcia aktualizuje obsah príspevku na pozícií id
        */
        bool UpdateContent(int id, std::string new_value){
            bool ret = false;
            //vyhľadávanie príspevku číslo id
            std::map<int,std::string>::iterator it = content.begin();
            for (it=content.begin(); it!=content.end(); ++it){
                if(it->first == id){
                    it->second = new_value;
                    ret = true;
                }
            }
            //vráti true ak bol príspevok nájdený a existuje, vráti false ak príspevok neexistuje
            return(ret);
        }
        /*
        *funkcia vymaže príspevok s číslom ida všetky ostatným príspevkom zmení id tak, aby išli v poradí
        */
        bool DeleteContent(int id){
            bool ret = false;
            std::string key;
            std::map<int,std::string>::iterator it = content.begin();
            
            for (it=content.begin(); it!=content.end(); ++it){
                if(it->first == id){
                    ret = true;
                    std::map<int, std::string>::iterator dupe = it;
                    //zmena id po vymazani prispevku
                    for(dupe = it; dupe != content.end();++dupe){
                        key = dupe->second;
                        it->second = key;
                        it = dupe;
                    }
                content.erase(it);
                i--;
                break;
                }
            }
            return(ret);
        }
        /*
        *vráti obsah nástenky
        */
        std::map<int, std::string> GetContent(){
            return(content);
        }
        
    private:
        std::map<int, std::string> content; //mapovanie obsahu na nastenku
        int i;

};

/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// CLASS LIST FOR LIST OF BOARDS /////////////////////////

class List{
    std::string name;
    std::vector<std::string> nastenky; // zoznam aktualnych nasteniek
    std::map<std::string, Board*> odkazy; //zoznam odkazov na nástenky
public:
    //koštruktor zoznamu
    List(std::string listname){
        name = listname;
    }
    //deštruktor zoznamu
    ~List(){
        name = "";
    }

    /*
    *skontroluje či nástenka s daným menom už existuje
    */
    bool BoardExists(std::string BoardName){
        for (unsigned int i = 0; i < nastenky.size(); i++){
            if(BoardName.compare(nastenky[i]) == 0){
                return true;
            }
            else continue;
        }
        return false;
    }

    /*  
    * pridá novú nástenku na zaznom 
    */
    void AddNewBoard(std::string nazov, Board* nastenka){
        /*if(std::regex_match(nazov, std::regex("[a-zA-Z0-9]+")) == false){
            //exit(1);
        }*/
        bool exists = BoardExists(nazov);
        if(exists == false){
            nastenky.push_back(nazov);
            odkazy.insert(std::pair<std::string, Board*>(nazov, nastenka));
        }
    }
    
    /*
    *vymaže nástenku zo zoznamu
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
    *vráti zoznam násteniek
    */
    std::vector<std::string> GetNastenky(){
        return(nastenky);
    }
    
    /*
    *vráti odkaz na konkrétnu nástenku s názvom key
    */
    Board* GetOdkaz(std::string key){
        bool exists = BoardExists(key);
        if( exists == true){
            std::map<std::string, Board*>::iterator it;
            it = odkazy.find(key);
            return(it->second);
        }
        else return(NULL);
    }
};

////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// ARGUMENTS PARSING //////////////////////////////////////////

int main(int argc, char *argv[]){
    char *portnum;
    /*
    *kontrola argumentov
    */
    if(argc > 3){
        std::cerr << "Too many arguments\n";
        return 0;
    }
    int opt;
    while ((opt = getopt(argc, argv, ":hp")) != -1) {
        switch (opt) {
            case 'h':
                std::cout << "Vitajte v programe isaserver.cpp - projekt do predmetu ISA.\nServer spustíte príkazom ./isaserver -p <port>\nNa serveri je možné vytvárať nástenky a ukldať do nej jednoriadkové alebo viacriadkové príspevky\n";
                exit(0);
            case 'p':
                continue;
            default:
                std::cerr << "Invalid arguments\n";
                std::cout << "Vitajte v programe isaserver.cpp - projekt do predmetu ISA.\nServer spustíte príkazom ./isaserver -p <port>\nNa serveri je možné vytvárať nástenky a ukldať do nej jednoriadkové alebo viacriadkové príspevky\n";
                exit(-1);
        }
    }
    
    if(argc == 3 && (strcmp(argv[1], "-p") == 0)){
            portnum = argv[optind];
    }
    
    
  
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////// SERVER CONNECTION ////////////////////////////////

    int fd;
    int sock;
    int leng;
    int size;
    int i;
    std::string method;
    std::string where;
    std::string name;
    int id;
    std::string ret;
    std::string text;
    std::string message;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char buffer[BUFFER+1] = {0};
    
    //vytvorenie objektu List pre zapisvanie a zapamatavanie nasteniek
    List* zoznam = new List("novy");
    
    //vytvorenie a nastavenie socketu
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        err(1,"could not create socket");
    }

    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(portnum));
    
    //spojí socket s portom
    if(bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0){
        err(1,"binding failed");
    }
    
    if(listen(fd, QUEUE) != 0){
        err(1, "Listen() not possible");
    }
    
    leng = sizeof(client);
    
    //čaká na klienta
    while(1){
        //pripojenie sa ku klientovi
        if((sock = accept(fd, (struct sockaddr *)&client, (socklen_t *)&leng)) == -1){
            err(1, "accept failed");
        }

        
        //prijímanie dát od klienta
        if((size = read(sock, buffer, BUFFER)) > 0){
            buffer[size] = 0;
        }
        
        
/////////////////////////////////////////////////////////////////////////////////
///////////////////// HEADER PARSING ////////////////////////////////////////////
        
        //char to string
        std::string buf(buffer);
        int iter = 0;
        //oddeli obsah správy na miesto znaku "\n\n"
        std::string delimiter = "\r\n\r\n";
        std::string token;
        size_t position = 0;
        
        //hľadá pozíciu znaku v načítanej správe v bufferi
        while((position = buf.find(delimiter)) != std::string::npos){
            token = buf.substr(0, position);
            size_t pos = buf.length();
            
            //ak je nájdený znak na konci správy,správa nemá žiadny obsah
                if((position + 4) < pos){
                    text = buf.substr((position +4), pos);
                    
                    //vo viacriadkovom príspevku nahradí reťazec "\n" novym riadkom
                    std::size_t found; 
                    while((found = text.find("\\n")) != std::string::npos){
                        text.replace(found, 2, "\n");
                    }
                }
                buf.erase(0, position + delimiter.length());
                iter++;
        }
        
        //oddelí prvý riadok hlavičky
        char* pch = strtok(buffer, "\r\n");
        
        //parsuje hlavicku
        std::string s(pch);
        std::smatch m;
        std::regex get("^GET\\s(\\/.+)\\sHTTP\\/1.1$");
        std::regex put("^PUT\\s\\/board\\/([a-zA-Z0-9]+)\\/([0-9]+)\\sHTTP\\/1.1$");
        std::regex post("^POST\\s(\\/.*)\\sHTTP\\/1.1$");
        std::regex delet("^DELETE\\s(\\/.*)\\sHTTP\\/1.1$");
        
        //HTTP požiadavka GET, preparsovanie a nastavenie premenných
        if (std::regex_search(s, m, get) == true){
            method = "GET";
            std::string s1 = m[1];
            std::smatch m1;
            std::regex board("^\\/board\\/([a-zA-Z0-9]+)$");
            std::regex boards("^\\/boards$");
            
            if(std::regex_search(s1, m1, board) == true){
                where = "board";
                name = m1[1];
            }
            else if(std::regex_search(s1, m1, boards)){
                where = "boards";
            }
            else ret = "404";
        }
        
        //HTTP požiadavka PUT preparsovanie a nastavenie premenných
        else if (std::regex_search(s, m, put) == true){
            method = "PUT";
            where = "board";
            name = m[1];
            std::string sid = m[2];
            std::stringstream stream(sid);
            id = 0;
            stream >> id;
        }
        
        //HTTP požiadavka POST preparsovanie a nastavenie premenných
        else if(std::regex_search(s, m, post) == true){
            method = "POST";
            std::string s1 = m[1];
            std::smatch m1;
            std::regex board("^\\/board\\/([a-zA-Z0-9]+)$");
            std::regex boards("^\\/boards\\/([a-zA-Z0-9]+)$");
            
            if(std::regex_search(s1, m1, board)){
                where = "board";
                name = m1[1];
            }
            else if(std::regex_search(s1, m1, boards)){
                where = "boards";
                name = m1[1];
            }
            else ret = "404";
        }
        
        //HTTP požiadavka DELETE preparsovanie a nastavenie premenných
        else if(std::regex_search(s, m, delet) == true){
            method = "DELETE";
            std::string s1 = m[1];
            std::smatch m1;
            std::regex boards("^\\/boards\\/([a-zA-Z0-9]+)$");
            std::regex board("^\\/board\\/([a-zA-Z0-9]+)\\/([0-9]+)$");
            
            if(std::regex_search(s1, m1, boards)){
                where = "boards";
                name = m1[1];
            }
            else if(std::regex_search(s1,m1, board)){
                where = "board";
                name = m1[1];
                std::string sid = m1[2];
                id = std::stoi(sid);
            }
            else{
             ret = "404 Not found";
             message = "Not found";
            }    
        }
        else{
         ret = "404 Not found";
         message = "Not found";
        } 
        
        
        
        ///////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////// FOR PARSED MESSAGE ////////////////////////////////////////////////
        
        //POST /boards/name
        if(method == "POST" && where == "boards"){
            //ak existuje nastenka s danym nazvom, vracia 409
            bool exist = zoznam->BoardExists(name);
            if(exist == true){
                    ret = "409 Conflict";
                    message = "Conflict";
            }
            //vytvori novu prazdnu nastenku typu Board a prida ju na zoznam nasteniek
            //kod uspecu je 201 pre metodu POST
            else{
                Board* new_board = new Board(name);
                zoznam->AddNewBoard(name, new_board);
                ret = "201 Created";
                message = "Created.";
            }
        }
        
        //GET /boards 
        else if(method == "GET" && where == "boards"){
            std::vector<std::string> nastenky = zoznam->GetNastenky();
            //ak ziadna nastenka neexistuje, kod uspechu je 404
            if(nastenky.empty()){
                ret = "404 Not found"; 
                message = "Not found.";   
            }
            //ak zoznam nasteniek nie je prazdny, kod uspechu je 200 
            else{
                
                std::stringstream sss;
                //std::vector<std::string> naste = zoznam->GetNastenky();
                for(unsigned int o =0; o<nastenky.size(); o++){
                    sss << nastenky[o] + "\n";
                }
                message = sss.str(); //text do spravy
                ret = "200 Ok";
                //message = "Ok.";
            } 
        }
        
        //DELETE /boards/name
        else if(method == "DELETE" && where == "boards"){
            //ak nastenka s danym menom neexistuje, vracia 404
            bool exist = zoznam->BoardExists(name);
            if(exist==false){
                ret = "404 Not found";
                message = "Not found.";
            }
            //odstrani nastenku zo zoznamu a nasledne aj samotnu nastenku
            else{
                Board* del = zoznam->GetOdkaz(name);
                zoznam->DeleteBoard(name);
                delete del;
                ret = "200 Ok";
                message = "Ok.";
            }
        }
        
        //GET /board/name
        else if(method == "GET" && where == "board"){
            bool exist = zoznam->BoardExists(name);
            //ak nastenka s danym menom neexistuje, vracia 404
            if(exist==false){
                ret = "404 Not found";
                message = "Not found";
            }
            //vrati obsah nastenky id
            else{
                
                Board* show = zoznam->GetOdkaz(name);
                std::map<int, std::string> obsah = show->GetContent();
                std::stringstream ss;
                std::map<int,std::string>::iterator it = obsah.begin();
                for (it=obsah.begin(); it!=obsah.end(); ++it){
                    ss << it->first << ". " << it->second << '\n';
                }
                message = "[";
                message += name;
                message += "]";
                message += "\n";
                message += ss.str();
                
            
                //message = ss.str();
                ret = "200 Ok";
                }
        }
        
        //POST /board/name
        else if(method == "POST" && where == "board"){
            //ak text ktory chceme vlozit je prazdny, vracia 400
            if(text.size() == 0){
                ret = "400 Bad request";
                message = "Bad request";
            }
            bool exist = zoznam->BoardExists(name);
            //ak nastenka s danym menom neexistuje, vracia 404
            if(exist==false){
                ret = "404 Not found";
                message = "Not found";
            }
            //prida novy prispevok na koniec nastenky
            else{
                Board* board = zoznam->GetOdkaz(name);
                board->AddContent(text);
                ret = "201 Created";
                message = "Created";
            }
        }
        
        //PUT /board/name/id 
        else if(method == "PUT" && where == "board"){
            //ak text ktory chceme vlozit je prazdny, vracia 400
            if(text.size() == 0){
                ret = "400 Bad request";
                message = "Bad request";
            }
            //ak nastenka s danym menom neexistuje, vracia 404
            bool exist = zoznam->BoardExists(name);
            if(exist==false){
                ret = "404 Not found";
                message = "Not found";
            }
            //zmeni obsah nastenky na prispevku id na hodnotu text
            else{
                Board* board = zoznam->GetOdkaz(name);
                bool result = board->UpdateContent(id, text);
                if(result == false){
                    ret = "404 Not found";
                    message = "Not found";
                }
                else{        
                    ret = "200 Ok";
                    message = "Ok";
                }
            }
        }
        
        //DELETE /board/name/id -
        else if(method == "DELETE" && where == "board"){
            bool exist = zoznam->BoardExists(name);
            //ak nastenka s danym menom neexistuje, vracia 404
            if(exist==false){
                ret = "404 Not found";
                message = "Not found";
            }
            //odstrani prispevok id na nastenke name
            else{
                Board* board = zoznam->GetOdkaz(name);
                bool result = board->DeleteContent(id);
                if(result == false){
                    ret = "404 Not found";
                    message = "Not found";
                }
                else{
                    ret = "200 Ok";
                    message = "Ok";
                }
            }
        }
        
        //vyrátanie dĺžky obsahu správy
        int m_l = message.length();
        std::ostringstream str1;
        str1 << m_l;
        std::string mess_len = str1.str();
        
        
        //zostavenie správy
        std::stringstream ss;
        ss  << "HTTP/1.1 " << ret << "\r\n" << "Content-Type: text/plain\r\nContent-Lenght: " << mess_len << "\r\n\r\n" << message << "\n";
        std::string send = ss.str();
        
        //vyrátanie veľkosti správy
        int l =send.length();
        char *message_array = new char[l +1];
        strcpy(message_array, send.c_str());
        
        //odoslanie odpovede klientovi
        size = strlen(send.c_str());
        i = write(sock, send.c_str(), size);
        if(i != size){
            err(1, "write() failed");
        }
        
        memset(buffer, 0, sizeof(BUFFER));
        //zatvorenie socketu a cakanie na dalsieho klienta
        close(sock);  
}

 return 0;
}
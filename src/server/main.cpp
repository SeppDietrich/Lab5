#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <string>

#define PORT 8080
#define MAX_CLIENTS 10

class User{
private:
    int socket;
    std::string Username;

public:
    User(int socket) {
        this->socket = socket;  
    }
    ~User() {}  

    std::string getUsernameString(){
        return Username;
    }
    
    bool authenticationSucces(std::string sentUsername) {  
        
        if(sentUsername.length()){
            return false;
        }
        Username = sentUsername;  
        std::cout<<"Client authenticated as : " <<Username<<std::endl;
        return true;
    }

    void handleClient() {
        char buffer[1024];

        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = read(socket, buffer, sizeof(buffer));
            if (bytesRead <= 0) {
                std::cout << "Client disconnected." << std::endl;
                break;
            }
            buffer[bytesRead] = '\0';
            
            // Trim any trailing newlines or whitespace
            std::string message(buffer);
            
            
            
            std::string fullMessage = parseMesage(message);
            
            std::cout << fullMessage << "\n";
            
            int bytesSent = send(socket, fullMessage.c_str(), fullMessage.size(), 0);  
            if (bytesSent < 0) {
                std::cerr << "Error sending message to client" << std::endl;
                break;
            }  
        }
        close(socket);
    }
private:

    std::string parseMesage(std::string message){
        message.erase(message.find_last_not_of(" \t\n\r\f\v") + 1);
        if(message[0]!='/'){
            std::string manual=
                "/auth [username] - authenticate \n"
                "/join [chatNumber] - join chat \n"
                "/message [message] -message sent into chat \n"
                "/leave -leave chat \n";
            
            return manual;
        }
        int posCommand =message.find(" ");
        std::string command=message.substr(0, posCommand);
        std::string data =message.substr(posCommand+1);
        switch(command){
            case '/join':
                if(authenticationSucces(data)){
                    std::string returnMessage="Succesfuly loged in as "+ Username+ "\n";
                }else{
                    return "Authentication Failed";
                }
            break;
            
            default:
                return command;
            break;
            
        }

    return "Unknown command";
};



int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed." << std::endl;
        return -1;
    }

    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        std::cerr << "Listen failed." << std::endl;
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket < 0) {
            std::cerr << "Accept failed." << std::endl;
            continue;
        }
        std::cout << "New client connected." << std::endl;

        User client(clientSocket);

        std::thread(&User::handleClient, &client).detach();
    }

    close(serverSocket);
    return 0;
}
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

class User
{
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
    
    bool authentication() {  
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(socket, buffer, sizeof(buffer), 0);  
        
        if (bytesRead <= 0) {
            std::cout << "Client disconnected." << std::endl;
            return false;  
        }
        
        Username = std::string(buffer, bytesRead);  
        return true;
    }

};


void handleClient(int clientSocket) {
    User client(clientSocket);
    char buffer[1024];
    if (!client.authentication()){
        return;
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(clientSocket, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }
        const* char Username =client.getUsernameString();
        buffer =Username.c_str() + " : " + buffer
        std::cout << client.getUsernameString()<<" : " << buffer << std::endl;
        send(clientSocket, buffer, bytesRead, 0);
    }
    close(clientSocket);
}

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

        std::thread(handleClient, clientSocket).detach();
    }

    close(serverSocket);
    return 0;
}
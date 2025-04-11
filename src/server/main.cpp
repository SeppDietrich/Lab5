#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>

#define PORT 8080
#define MAX_CLIENTS 10

class User;

class ChatRoom{
private:
    std::vector<User*> chaters;
    int id;
    ChatRoom(int id){
        this->id=id;
    }
    ~ChatRoom();


public:
    int getId(){return id;}
    void addChater(User* user){
        chaters.push_back(user);
    }
    void broadCastInRoom(std::string Username, std::string message){
        std::string fullMessage=Username+" : "+message+"\n";
        for (User* user : chaters) {
            user->sendMessage(fullMessage);
        }
    }
};
class RoomHandler {
private:
    std::vector<ChatRoom> rooms;

    void createRoom(int roomId) {
        rooms.emplace_back(roomId); 

    }

public:
    
    void joinRoom(const User* user, int roomId) {
        
        bool found =false;
        findRoom:
        ChatRoom* roomToJoin=nullptr;
        for(int i=0;i<rooms.length;i++){
            if(rooms[i].getId()==roomId){
                found=true;
            }
        }
        if(!found){
            createRoom(roomId);
            goto findRoom;
        }

        roomToJoin.addChater(user);
    }
};

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

    void sendMessage(const std::string& message){
        int bytesSent = send(socket, message.c_str(), message.size(), 0);  
        if (bytesSent < 0) {
            std::cerr << "Error sending message to client" << std::endl;
        }  
    }
    
    bool authenticationSuccess(std::string sentUsername) {  
        
        if(sentUsername.length()>100){
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
            
            //std::cout << fullMessage << "\n";
            
            sendMessage(fullMessage); 
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
        int spacePos = message.find(' ');
        std::string command = message.substr(0, spacePos);
        std::string data = (spacePos != std::string::npos) ? message.substr(spacePos + 1) : "";

        
        switch(command[1]) {
            case 'a': // /auth
                if(authenticationSuccess(data)) {
                    return "Successfully logged in as " + data + "\n";
                }
                return "Authentication Failed";
                
            case 'j': // /join
                return "Joining chat: " + data;
                
            case 'm': // /message
                return "Sending message: " + data;
                
            case 'l': // /leave
                return "Leaving current chat";
                
            default:
                return "Unknown command: " + command;
        }

        return "Unknown command";
    }
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
        std::cout << "New client connected: "<< std::endl;

        User client(clientSocket);

        std::thread(&User::handleClient, &client).detach();
    }

    close(serverSocket);
    return 0;
}
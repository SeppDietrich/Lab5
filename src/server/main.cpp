#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include "RoomHandler.h"  // Own header first
#include "ChatRoom.h"     // For ChatRoom class
#include "User.h"         // For User class

#define PORT 8080
#define MAX_CLIENTS 10


class RoomHandler {
private:
    std::vector<ChatRoom> rooms;

    void createRoom(int roomId) {
        rooms.emplace_back(roomId); 

    }

public:
    
    void joinRoom(User* user, int roomId) {
        ChatRoom* roomToJoin = nullptr;
        
        // Find existing room
        for(auto& room : rooms) {
            if(room.getId() == roomId) {
                roomToJoin = &room;
                break;
            }
        }
        
        // Create new room if not found
        if(!roomToJoin) {
            rooms.emplace_back(roomId);
            roomToJoin = &rooms.back();
        }
        
        roomToJoin->addChater(user);

    }
    
};

class ChatRoom{
private:
    std::vector<User*> chaters;
    int id;
    


public:
    ChatRoom(int id){
        this->id=id;
    }
    
    ~ChatRoom() = default;
    int getId(){return id;}
    void addChater(User* user){
        chaters.push_back(user);
        user->setCurrentRoom(this);
    }
    void broadCastInRoom(std::string Username, std::string message){
        std::string fullMessage=Username+" : "+message+"\n";
        for(int i=0;i<chaters.size();i++){
            chaters[i]->sendMessage(fullMessage);
        }
       
    }
};

class User{
private:
    RoomHandler* roomHandler;
    int socket;
    std::string Username;
    ChatRoom* currentRoom;

public
    User(int socket, RoomHandler& rh) : 
        socket(socket), 
        roomHandler(rh),
        currentRoom(nullptr) {}
    ~User() {}  

    ChatRoom* getCurrentRoom() const {
        return currentRoom;
    }

    void User::setCurrentRoom(ChatRoom* room) {
        currentRoom = room;
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
            
            
            
            std::string fullMessage = parseMessage(message);
            
            //std::cout << fullMessage << "\n";
            
            sendMessage(fullMessage); 
        }
        close(socket);
    }
private:

    std::string parseMessage(std::string message){
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
            case 'a':{ // /auth
                if(authenticationSuccess(data)) {
                    return "Successfully logged in as " + data + "\n";
                }
                return "Authentication Failed";
            }
                
            case 'j':{ // /join
                int roomId=std::stoi(data);
                roomHandler.joinRoom(this, data);

                return "Joining chat: " + data;
            }
            case 'm':{ // /message
                currentRoom->broadCastInRoom(Username, data);
                return "Sending message: " + data;
            }
            case 'l':{ // /leave
                return "Leaving current chat";
            }
            default:{
                return "Unknown command: " + command;
            }
        }

        return "Unknown command";
    }
};
RoomHandler roomHandler;


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

        User client(clientSocket, &roomHandler);

        std::thread(&User::handleClient, &client).detach();
    }

    close(serverSocket);
    return 0;
}
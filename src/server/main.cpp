#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include "User.h"

#define PORT 8080
#define MAX_CLIENTS 10



class ChatRoom{
private:
    std::vector<User*> chaters;
    int id;
    


public:
    ChatRoom(int id){
        this->id=id;
    }
    ~ChatRoom();
    int getId(){return id;}
    void addChater(User* user){
        chaters.push_back(user);
    }
    void broadCastInRoom(std::string Username, std::string message){
        std::string fullMessage=Username+" : "+message+"\n";
        for(int i=0;i<chaters.size();i++){
            chaters[i]->sendMessage(fullMessage);
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
    
    void joinRoom(User* user, int roomId) {
        
        bool found =false;
        findRoom:
        ChatRoom* roomToJoin=nullptr;
        for(int i=0;i<rooms.size();i++){
            if(rooms[i].getId()==roomId){
                found=true;
            }
        }
        if(!found){
            createRoom(roomId);
            goto findRoom;
        }

        roomToJoin->addChater(user);
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

        User client(clientSocket);

        std::thread(&User::handleClient, &client).detach();
    }

    close(serverSocket);
    return 0;
}
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
#include "ChatRoom.h"

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

        User client(clientSocket, roomHandler);

        std::thread(&User::handleClient, &client).detach();
    }

    close(serverSocket);
    return 0;
}
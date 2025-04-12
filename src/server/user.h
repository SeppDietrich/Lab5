#include <cstring>
#include <unistd.h>
#include <string>


#pragma once


class ChatRoom;
class RoomHandler;

class User {
private:
    int socket;
    std::string Username;
    ChatRoom* currentRoom;
    RoomHandler& roomHandler;  

public:
    
    User(int socket, RoomHandler& rh);
    ~User();
    
    
    void setCurrentRoom(ChatRoom* room);
    ChatRoom* getCurrentRoom() const;
    void sendMessage(const std::string& message);
    bool authenticationSuccess(std::string sentUsername);
    void handleClient();

private:
    std::string parseMessage(std::string message);
};
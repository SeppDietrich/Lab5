#pragma once
#include <string>
#include "ChatRoom.h"
#include "RoomHandler.h"


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
#pragma once
#include <string>

class ChatRoom;
class RoomHandler;
extern RoomHandler roomHandler; 

class User {
private:
    int socket;
    RoomHandler& roomHandler;
    std::string Username;
    ChatRoom* currentRoom;

public:
    User(int socket);
    ~User();
    
    ChatRoom* getCurrentRoom() const;
    void setCurrentRoom(ChatRoom* room);
    
    void sendMessage(const std::string& message);
    bool authenticationSuccess(std::string sentUsername);
    void handleClient();

private:
    std::string parseMesage(std::string message);
};
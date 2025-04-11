#pragma once
#include <string>

// Forward declaration instead of including ChatRoom.h
class ChatRoom;

class User {
private:
    int socket;
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
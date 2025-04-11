#pragma once
#include <string>

class ChatRoom;
class RoomHandler;


class User {
private:
    int socket;
    RoomHandler& roomHandler;
    std::string Username;
    ChatRoom* currentRoom;

public:
    User(int socket, RoomHandler& rh);
    ~User();
    
    ChatRoom* getCurrentRoom() const;
    void setCurrentRoom(ChatRoom* room);
    
    void sendMessage(const std::string& message);
    bool authenticationSuccess(std::string sentUsername);
    void handleClient();

private:
    std::string parseMesage(std::string message);
};
#pragma once
#include <vector>
#include <string>

// Forward declaration instead of including User.h
class User;

class ChatRoom {
private:
    std::vector<User*> chaters;
    int id;
    
public:
    ChatRoom(int id);
    ~ChatRoom();
    
    int getId();
    void addChater(User* user);
    void broadCastInRoom(std::string Username, std::string message);
};
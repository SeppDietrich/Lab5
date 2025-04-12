#pragma once
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
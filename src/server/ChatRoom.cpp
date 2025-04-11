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
#pragma once

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
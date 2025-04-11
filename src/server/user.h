#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include "ChatRoom.h"


#pragma once
#define PORT 8080
#define MAX_CLIENTS 10

class User{
private:
    int socket;
    std::string Username;

public:
    User(int socket) {
        this->socket = socket;  
    }
    ~User() {}  

    std::string getUsernameString(){
        return Username;
    }

    void sendMessage(const std::string& message){
        int bytesSent = send(socket, message.c_str(), message.size(), 0);  
        if (bytesSent < 0) {
            std::cerr << "Error sending message to client" << std::endl;
        }  
    }
    
    bool authenticationSuccess(std::string sentUsername) {  
        
        if(sentUsername.length()>100){
            return false;
        }
        Username = sentUsername;  
        std::cout<<"Client authenticated as : " <<Username<<std::endl;
        return true;
    }

    void handleClient() {
        char buffer[1024];

        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = read(socket, buffer, sizeof(buffer));
            if (bytesRead <= 0) {
                std::cout << "Client disconnected." << std::endl;
                break;
            }
            buffer[bytesRead] = '\0';

            
            // Trim any trailing newlines or whitespace
            std::string message(buffer);
            
            
            
            std::string fullMessage = parseMesage(message);
            
            //std::cout << fullMessage << "\n";
            
            sendMessage(fullMessage); 
        }
        close(socket);
    }
private:

    std::string parseMesage(std::string message){
        message.erase(message.find_last_not_of(" \t\n\r\f\v") + 1);
        if(message[0]!='/'){
            std::string manual=
                "/auth [username] - authenticate \n"
                "/join [chatNumber] - join chat \n"
                "/message [message] -message sent into chat \n"
                "/leave -leave chat \n";
            
            return manual;
        }
        int spacePos = message.find(' ');
        std::string command = message.substr(0, spacePos);
        std::string data = (spacePos != std::string::npos) ? message.substr(spacePos + 1) : "";

        
        switch(command[1]) {
            case 'a': // /auth
                if(authenticationSuccess(data)) {
                    return "Successfully logged in as " + data + "\n";
                }
                return "Authentication Failed";
                
            case 'j': // /join
            	int roomId=eval(data);
            	roomHandler.joinRoom(this, data);

                return "Joining chat: " + data;
                
            case 'm': // /message
                return "Sending message: " + data;
                
            case 'l': // /leave
                return "Leaving current chat";
                
            default:
                return "Unknown command: " + command;
        }

        return "Unknown command";
    }
};
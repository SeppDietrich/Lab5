#include "ChatRoom.h"
#include "User.h"  // Full definition needed here


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
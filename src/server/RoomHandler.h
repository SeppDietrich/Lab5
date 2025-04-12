
#pragma once
#include <vector>




class RoomHandler {
private:
    std::vector<ChatRoom> rooms;
    void createRoom(int roomId);

public:
    void joinRoom(User* user, int roomId);
};
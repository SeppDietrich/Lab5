#pragma once
#include <vector>

class ChatRoom;
class User;

class RoomHandler {
private:
    std::vector<ChatRoom> rooms;
    void createRoom(int roomId);

public:
    void joinRoom(User* user, int roomId);
};
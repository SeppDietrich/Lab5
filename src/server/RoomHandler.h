
#include <vector>
#include "User.h"
#pragma once
class ChatRoom;


class RoomHandler {
private:
    std::vector<ChatRoom> rooms;
    void createRoom(int roomId);

public:
    void joinRoom(User* user, int roomId);
};
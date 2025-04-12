#pragma once
#include <vector>
#include "User.h"

class ChatRoom;


class RoomHandler {
private:
    std::vector<ChatRoom> rooms;
    void createRoom(int roomId);

public:
    void joinRoom(User* user, int roomId);
};
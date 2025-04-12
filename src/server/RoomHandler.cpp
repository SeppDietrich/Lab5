#include "RoomHandler.h"
#include "ChatRoom.h"  // Full definitions
#include "User.h"

// Implement methods...
// class RoomHandler {
// private:
//     std::vector<ChatRoom> rooms;

//     void createRoom(int roomId) {
//         rooms.emplace_back(roomId); 

//     }

// public:
    
//     void joinRoom(User* user, int roomId) {
//         ChatRoom* roomToJoin = nullptr;
        
//         // Find existing room
//         for(auto& room : rooms) {
//             if(room.getId() == roomId) {
//                 roomToJoin = &room;
//                 break;
//             }
//         }
        
//         // Create new room if not found
//         if(!roomToJoin) {
//             rooms.emplace_back(roomId);
//             roomToJoin = &rooms.back();
//         }
        
//         roomToJoin->addChater(user);

//     }
    
// };
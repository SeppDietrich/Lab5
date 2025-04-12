#!/bin/bash
git pull

# g++ -std=c++11 src/server/main.cpp src/server/User.cpp src/server/ChatRoom.cpp src/server/RoomHandler.cpp -o server -lpthread
g++ -std=c++17 -Wall -Wextra -I./include src/server/*.cpp src/server/main.cpp -o server
./server

rm server
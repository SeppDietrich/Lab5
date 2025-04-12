#!/bin/bash
git pull

g++ -std=c++11 src/server/main.cpp src/server/User.cpp src/server/ChatRoom.cpp src/server/RoomHandler.cpp -o server -lpthread

./server

rm server
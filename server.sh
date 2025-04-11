#!/bin/bash
git pull
chmod +x server.sh

g++ -o server src/server/main.cpp

./server

rm server
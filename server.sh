#!/bin/bash
git pull

g++ -o server src/server/main.cpp

./server

rm server
#!/bin/bash

g++ -E src/server/User.cpp > user_preprocessed.txt

cat user_preprocessed
rm user_preprocessed
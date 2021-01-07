cmake_minimum_required(VERSION 3.17)
project(BoardBuilder)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BoardBuilder
        Board.cpp
        Board.h
        main.cpp
        NewWord.cpp
        NewWord.h)

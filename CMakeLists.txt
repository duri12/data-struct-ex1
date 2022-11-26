cmake_minimum_required(VERSION 3.21.1)
project(data_struct_ex1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(data_struct_ex1
        main23a1.cpp
        wet1util.h
        worldcup23a1.cpp
        Player.cpp
        worldcup23a1.h Team.h Player.h ActiveTeam.h AvlTree.h)

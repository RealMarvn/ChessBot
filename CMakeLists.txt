cmake_minimum_required(VERSION 3.27)
project(pk1_pruefung)

set(CMAKE_CXX_STANDARD 23)

add_executable(pk1_pruefung main.cpp
        manager/board_manager.cpp
        manager/board_manager.h
        manager/pieces/pawn.h
        manager/pieces/pawn.cpp
        manager/pieces/king.cpp
        manager/pieces/king.h
        manager/pieces/bishop.cpp
        manager/pieces/bishop.h
        manager/pieces/rook.cpp
        manager/pieces/rook.h
        manager/pieces/queen.cpp
        manager/pieces/queen.h
        manager/pieces/knight.cpp
        manager/pieces/knight.h
)

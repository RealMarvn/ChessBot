cmake_minimum_required(VERSION 3.27)
project(pk1_pruefung)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(pk1_pruefung main.cpp
        manager/board_manager.cpp
        manager/board_manager.h
        manager/move_gen.cpp
        manager/move_gen.h
        manager/piece_movements.h
        manager/piece_movements.cpp
        manager/misc/move.h
        manager/misc/PieceType.h
        manager/misc/PieceType.cpp
        engine/chess_bot.cpp
        engine/chess_bot.h
)

cmake_minimum_required(VERSION 3.27)
project(pk1_pruefung)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(pk1_pruefung main.cpp
        manager/board.cpp
        manager/board.h
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

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

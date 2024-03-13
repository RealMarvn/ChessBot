cmake_minimum_required(VERSION 3.27)
project(pk1_pruefung)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

enable_testing()

add_executable(pk1_pruefung main.cpp
        engine/board.cpp
        engine/board.h
        engine/movement/move_gen.cpp
        engine/movement/move_gen.h
        engine/movement/piece_movements.h
        engine/movement/piece_movements.cpp
        engine/misc/move.h
        engine/misc/piece.h
        engine/misc/piece.cpp
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

# Add your test source file
add_executable(tests tests/tests.cpp
        engine/board.cpp
        engine/board.h
        engine/movement/move_gen.cpp
        engine/movement/move_gen.h
        engine/movement/piece_movements.h
        engine/movement/piece_movements.cpp
        engine/misc/move.h
        engine/misc/piece.h
        engine/misc/piece.cpp
        engine/chess_bot.cpp
        engine/chess_bot.h
)

# Link the Google libraries to your test executable
target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

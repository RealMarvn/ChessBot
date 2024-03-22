# Makefile for gtest

GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /usr/local/include

G++ = g++

G++_FLAGS = -std=c++17 -c -Wall -Wextra -Wpedantic -O3 -I $(GOOGLE_TEST_INCLUDE)
LD_FLAGS = -L /usr/local/lib -l $(GOOGLE_TEST_LIB) -l pthread

OBJECTS = chess_game.o chess_bot.o board.o move_gen.o tests.o
TARGET = Test

vpath chess_game.cpp ../engine
vpath chess_bot.cpp ../engine
vpath board.cpp ../engine
vpath move_gen.cpp ../engine/movement

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) $(LD_FLAGS)

%.o : %.cpp
	$(G++) $(G++_FLAGS) $<

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean

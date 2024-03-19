# Makefile for chess engine

G++ = g++

G++_FLAGS = -std=c++17 -c -Wall -Wextra -Wpedantic -O3
LD_FLAGS = -L /usr/local/lib -l pthread

OBJECTS = chess_game.o chess_bot.o board.o move_gen.o piece.o main.o
TARGET = ChessGame

vpath move_gen.cpp ./movement
vpath piece.cpp ./misc

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) $(LD_FLAGS)

%.o : %.cpp
	$(G++) $(G++_FLAGS) $<

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean

//
// Created by Marvin Becker on 15.12.23.
//

#pragma once

#include "./pieces/bishop.h"
#include "./pieces/king.h"
#include "./pieces/knight.h"
#include "./pieces/pawn.h"
#include "./pieces/queen.h"
#include "./pieces/rook.h"
#include <vector>

enum Player_type { WHITE, BLACK };

struct Piece {
  char figure;
  bool moved;
};

struct Move {
  int position;
  int old_position;
  Piece figure;
  Piece capturedFigure;
};

class BoardManager {

public:
  Player_type player;
  Piece board[65]{};
  std::vector<Move> moves;

  BoardManager();
  bool movePiece(char fig, int x, int y, int move_x, int move_y, bool capture, char promotion_figure);
  bool popLastMove();
  void printCurrentBoard(bool legacy);

private:
  void saveMove(int movePosition, int position);
  bool canMove(char fig, int x, int y, int move_x, int move_y, bool capture);
  static bool isPathClear(int startX, int startY, int endX, int endY,
                          Piece board[65]);
  static int calculatePosition(int x, int y);
};

//
// Created by Marvin Becker on 15.12.23.
//

#pragma once

#include "./pieces/pawn.h"
#include "./pieces/king.h"
#include "./pieces/bishop.h"
#include "./pieces/rook.h"
#include "./pieces/queen.h"
#include "./pieces/knight.h"
#include <vector>

enum player_type { WHITE, BLACK };

struct piece {
  char figure;
  bool moved;
};

class BoardManager {

public:
  player_type player;
  piece board[65]{};
  std::vector<piece[65]> timeline;

  BoardManager();
  void movePiece(char fig, int x, int y, int move_x, int move_y, bool capture);
  bool popLastMove();
  void printCurrentBoard();
  static bool isPathClear(int startX, int startY, int endX, int endY,
                          piece board[65]);
  static int calculatePosition(int x, int y);

private:
  bool canMove(char fig, int x, int y, int move_x, int move_y, bool caputre);
};

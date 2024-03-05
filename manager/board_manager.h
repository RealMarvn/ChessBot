//
// Created by Marvin Becker on 15.12.23.
//

#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "./move_gen.h"
#include "misc/move.h"
#include "misc/piece.h"

enum player_type { WHITE, BLACK };

class BoardManager {
 public:
  player_type player;
  piece board[65]{};
  std::vector<Move> moves;

  explicit BoardManager();
  bool movePiece(char fig, int x, int y, int move_x, int move_y, bool capture,
                 char promotion_figure);
  void makeMove(Move move);
  bool popLastMove();
  void printCurrentBoard();
  void readFen(std::string input);
  bool isKingInCheck(bool isWhite);
  void printPossibleMoves(char fig, int x, int y);

 private:
  int getPossibleMoves(int x, int y);
  bool isCheckMate(bool isWhite);
  bool isWhiteKingInDanger();
  bool isBlackKingInDanger();
  void saveMove(int movePosition, int position);
  bool canMove(char fig, int x, int y, int move_x, int move_y, bool capture);
  static bool isPathClear(int startX, int startY, int endX, int endY,
                          const piece board[65]);
};

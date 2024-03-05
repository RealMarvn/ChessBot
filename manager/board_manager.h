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

#include "./move.h"
#include "./piece.h"
#include "./move_gen.h"

enum player_type { WHITE, BLACK };
static int calculatePosition(int x, int y);

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
  static bool isWhitePiece(piece piece);
  void saveMove(int movePosition, int position);
  bool canMove(char fig, int x, int y, int move_x, int move_y, bool capture);
  static bool isPathClear(int startX, int startY, int endX, int endY,
                          const piece board[65]);
};

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

#include "./pieces/bishop.h"
#include "./pieces/king.h"
#include "./pieces/knight.h"
#include "./pieces/pawn.h"
#include "./pieces/queen.h"
#include "./pieces/rook.h"

enum player_type { WHITE, BLACK };
enum piece { WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK, EMPTY = 14 };

struct Move {
  int position;
  int old_position;
  piece figure;
  piece capturedFigure;
};

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
  std::map<piece, char> pieceToCharMap = {
      {WP, 'P'}, {WN, 'N'}, {WB, 'B'},   {WR, 'R'}, {WQ, 'Q'},
      {WK, 'K'}, {BP, 'p'}, {BN, 'n'},   {BB, 'b'}, {BR, 'r'},
      {BQ, 'q'}, {BK, 'k'}, {EMPTY, ' '}};
  int getPossibleMoves(int x, int y);
  bool isCheckMate(bool isWhite);
  bool isWhiteKingInDanger();
  bool isBlackKingInDanger();
  static bool isWhitePiece(piece piece);
  piece findKeyByValue(char value);
  void saveMove(int movePosition, int position);
  bool canMove(char fig, int x, int y, int move_x, int move_y, bool capture);
  static bool isPathClear(int startX, int startY, int endX, int endY,
                          const piece board[65]);
};

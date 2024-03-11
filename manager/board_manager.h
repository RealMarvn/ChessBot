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

#include "./piece_movements.h"
#include "misc/move.h"
#include "misc/piece.h"

enum player_type { WHITE, BLACK };

struct board_setting {
  int epSquare{100};
  bool whiteQueenSide{false};
  bool whiteKingSide{false};
  bool blackQueenSide{false};
  bool blackKingSide{false};
};

class BoardManager {
 public:
  player_type player;
  std::array<piece, 65> board{};
  std::vector<Move> moves;
  std::vector<board_setting> history;

  board_setting boardSettings;

  explicit BoardManager();
  bool movePiece(char fig, int x, int y, int move_x, int move_y, bool capture,
                 char promotion_figure);
  void makeMove(Move move);
  Move generateMove(int position, int moveToPosition, piece promotionPiece,
                    MoveType moveType);
  bool popLastMove();
  void printCurrentBoard();
  void readFen(std::string input);
  bool isKingInCheck(bool isWhite);
  void printPossibleMoves(char fig, int x, int y);

 private:
  void handleCastelingPermissions(Move& move);
  int getPossibleMoves(int x, int y);
  bool isCheckMate(bool isWhite);
  bool isWhiteKingInDanger();
  bool isBlackKingInDanger();
  bool canMove(char fig, int x, int y, int move_x, int move_y, bool capture);
  static bool isPathClear(int startX, int startY, int endX, int endY,
                          std::array<piece, 65> board);
};

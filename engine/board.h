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

#include "./misc/board_settings.h"
#include "./misc/move.h"
#include "./misc/piece.h"
#include "./movement/piece_movements.h"

enum player_type { WHITE, BLACK };

inline int calculatePosition(int x, int y) { return ((y - 1) * 8 + x) - 1; }

class Board {
 private:
  std::array<Piece, 64> board;

 public:
  player_type player;
  std::vector<Move> moves;
  std::vector<board_setting> history;

  board_setting boardSettings;

  Piece& operator[](int index) { return board[index]; }

  explicit Board() : player(WHITE), board{Piece()} {
    readFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  };

  bool isKingInCheck(bool pieceColor);
  bool isSquareAttacked(std::pair<int, int> square, bool pieceColor);
  bool movePiece(char fig, int x, int y, int move_x, int move_y, bool capture, char promotion_figure);
  void makeMove(Move move);
  Move generateMove(int position, int moveToPosition, Piece promotionPiece, MoveType moveType);
  bool popLastMove();
  void printCurrentBoard();
  void readFen(std::string input);
  bool isCheckMate(bool isWhite);
  //  void printPossibleMoves(char fig, int x, int y);

 private:
  void handleCastlingPermissions(Move& move);
  bool canMove(char fig, int x, int y, int move_x, int move_y, bool capture);
  static bool isPathClear(int startX, int startY, int endX, int endY, const std::array<Piece, 64>& board);
};

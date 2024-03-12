//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <map>

enum PieceType { WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK, EMPTY = 14 };

class Piece {

 public:
  Piece(PieceType piece);
  Piece();
  PieceType pieceType;
  bool isWhite() const;
  char toChar() const;
  int getMaterialValue() const;

 private:
  static std::map<PieceType, char> pieceToCharMap;
  static int pieceValue[6];
};

extern std::map<PieceType, char> pieceToCharMap;

Piece findKeyByValue(char value);

int calculatePosition(int x, int y);
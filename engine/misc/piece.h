//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <map>

enum PieceType { WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK, EMPTY };

class Piece {
 public:
  explicit Piece(PieceType piece) : pieceType{piece} {};
  Piece() : pieceType{EMPTY} {};
  PieceType pieceType{EMPTY};
  [[nodiscard]] inline bool isWhite() const { return (pieceType < BP); };
  [[nodiscard]] inline char toChar() const {
    return pieceToChar[pieceType];
  };
  [[nodiscard]] int getMaterialValue() const {
    if (pieceType == EMPTY) return 0;
    return ((isWhite() ? 1 : -1) * pieceValue[pieceType]);
  };

 private:
  static int pieceValue[6];
  static char pieceToChar[13];
};

extern std::map<PieceType, char> pieceToCharMap;

Piece findKeyByValue(char value);
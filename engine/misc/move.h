//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <array>
#include <cassert>
#include <sstream>
#include <string>
#include <utility>

#include "./piece.h"

#define MAX_MOVES 218

enum MoveType { NORMAL, EN_PASSANT, PROMOTION, CASTLING };

struct Move {
  int moveSquare{0};
  int square{0};
  Piece movingPiece;
  Piece capturedPiece;
  Piece promotionPiece;
  MoveType moveType{NORMAL};

  [[nodiscard]] std::string convertToXandY() const {
    std::ostringstream out;
    out << static_cast<char>((square) % 8 + 'a') << (square) / 8 + 1 << static_cast<char>((moveSquare) % 8 + 'a')
        << (moveSquare) / 8 + 1;
    return out.str();
  }

  bool operator==(const Move& other) const {
    return moveSquare == other.moveSquare && square == other.square && movingPiece.pieceType == other.movingPiece.pieceType
           && capturedPiece.pieceType == other.capturedPiece.pieceType && promotionPiece.pieceType == other.promotionPiece.pieceType && moveType == other.moveType;
  }
};

class PseudoLegalMoves {
  std::array<Move, MAX_MOVES> move_list{};
  int index = 0;

 public:
  using iterator = std::array<Move, MAX_MOVES>::iterator;
  iterator begin() { return move_list.begin(); }
  iterator end() { return move_list.begin() + index; }
  Move& operator[](int number) {
    assert(number < index);
    return move_list[number];
  }

  void push_back(const Move& mv) {
    assert(index < MAX_MOVES);
    move_list[index] = mv;
    ++index;
  }

  bool contains(const Move& mv) {
    return std::find(begin(), end(), mv) != end();
  }
};
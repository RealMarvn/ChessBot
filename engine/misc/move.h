//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <array>
#include <cassert>
#include <sstream>
#include <string>
#include <utility>

#include "./PieceType.h"

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
    out << static_cast<char>((square - 1) % 8 + 'a') << (square - 1) / 8 + 1
        << static_cast<char>((moveSquare - 1) % 8 + 'a')
        << (moveSquare - 1) / 8 + 1;
    return out.str();
  }
};

struct AllPseudoMoves {
  std::array<Move, MAX_MOVES> move_list{};
  int index = 0;

  void push_back(const Move& mv) {
    assert(index < MAX_MOVES);
    move_list[index] = mv;
    ++index;
  }
};
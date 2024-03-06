//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <array>
#include <cassert>
#include <utility>
#include <string>
#include <sstream>

#include "./piece.h"

#define MAX_MOVES 218

struct Move {
  int moved_position;
  int original_position;
  piece movedPiece;
  piece capturedPiece;

  [[nodiscard]] std::string convertToXandY() const {
    std::ostringstream out;
    out << static_cast<char>((original_position - 1) % 8 + 'a') << (original_position - 1) / 8 + 1 << static_cast<char>((moved_position - 1) % 8 + 'a') << (moved_position - 1) / 8 + 1 ;
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
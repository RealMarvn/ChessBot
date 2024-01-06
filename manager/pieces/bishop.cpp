//
// Created by Marvin Becker on 05.01.24.
//

#include "./bishop.h"

bool canBishopMove(int x, int y, int move_x, int move_y) {
  return std::abs(y - move_y) == std::abs(x - move_x);
}
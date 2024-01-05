//
// Created by Marvin Becker on 05.01.24.
//

#include "queen.h"

bool canQueenMove(int x, int y, int move_x, int move_y) {
  if (x == move_x && y != move_y) return true;
  if (y == move_y && x != move_x) return true;
  if (std::abs(y - move_y) == std::abs(x - move_x)) return true;
  return false;
}

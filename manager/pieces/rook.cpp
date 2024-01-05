//
// Created by Marvin Becker on 05.01.24.
//

#include "rook.h"

bool canRookMove(int x, int y, int move_x, int move_y) {
  if (x == move_x && y != move_y) return true;
  if (y == move_y && x != move_x) return true;

  return false;
}

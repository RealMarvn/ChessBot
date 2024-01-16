//
// Created by Marvin Becker on 16.01.24.
//

#include "mimic.h"

bool canMimicMove(int x, int y, int move_x, int move_y) {
  if ((y == move_y && abs(x - move_x) == 1))
    return true;
  if ((x == move_x && abs(y - move_y) == 1))
    return true;
  if (std::abs(y - move_y) == std::abs(x - move_x) && std::abs(y - move_y) == 1)
    return true;

  return false;
}
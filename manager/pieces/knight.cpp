//
// Created by Marvin Becker on 05.01.24.
//

#include "knight.h"

bool canKnightMove(int x, int y, int move_x, int move_y) {
  int rowDiff = std::abs(y - move_y);
  int colDiff = std::abs(x - move_x);

  return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}

//
// Created by Marvin Becker on 04.01.24.
//

#include "./pawn.h"

bool canPawnMove(int x, int y, int move_x, int move_y, bool capture, bool hasMoved, bool isWhite) {
  int rowDiff = std::abs(move_y - y);
  int colDiff = std::abs(move_x - x);

  if (rowDiff > 0) {

     if (isWhite && move_y < y || !isWhite && move_y > y) {
       return false;
     }

    if (colDiff == 0 && rowDiff == 1 && !capture) {
      return true;
    }

    if (colDiff == 0 && rowDiff == 2 && !hasMoved) {
      return true;
    }

    if (colDiff == 1 && rowDiff == 1 && capture) {
      return true;
    }
  }

  return false;
}

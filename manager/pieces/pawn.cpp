//
// Created by Marvin Becker on 04.01.24.
//

#include "./pawn.h"

bool canPawnMove(int x, int y, int move_x, int move_y, bool capture, bool isWhite) {
  int rowDiff = std::abs(move_y - y);
  int colDiff = std::abs(move_x - x);

  if (rowDiff > 0) {
    if ((isWhite && move_y < y) || (!isWhite && move_y > y)) {
      return false;
    }

    if (colDiff == 0 && rowDiff == 1 && !capture) {
      return true;
    }

    if (colDiff == 0 && rowDiff == 2 && !capture) {
      if (isWhite && y == 2 || !isWhite && y == 7) {
        return true;
      }
    }

    if (colDiff == 1 && rowDiff == 1 && capture) {
      return true;
    }
  }

  return false;
}

bool canPawnPromote(bool isWhite, char fig, char promotion_figure,
                    int movePosition) {
  if (tolower(fig) == 'p') {
    if ((movePosition >= 56 && movePosition <= 64) ||
        (movePosition >= 1 && movePosition <= 8)) {
      if ((isWhite && isupper(promotion_figure)) ||
          (!isWhite && islower(promotion_figure))) {
        switch (tolower(promotion_figure)) {
          case 'r':
          case 'n':
          case 'q':
          case 'b':
            return true;
        }
      }
    }
  }
  return false;
}

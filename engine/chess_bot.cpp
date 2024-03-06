//
// Created by Marvin Becker on 10.02.24.
//

#include "chess_bot.h"

int getPieceValue(piece piece) {
  switch (std::tolower(pieceMap[piece])) {
    case 'p':
      return 100;
    case 'n':
      return 300;
    case 'b':
      return 350;
    case 'r':
      return 500;
    case 'q':
      return 900;
    case 'k':
      return 1000;
    default:
      return 0;
  }
}

bool isWhitePiece(piece piece) {
  switch (piece) {
    case WP:
    case WN:
    case WB:
    case WR:
    case WQ:
    case WK:
      return true;
    default:
      return false;
  }
}

int evaluateMaterialValue(std::array<piece, 65> board) {
  int value = 0;
  for (int y = 1; y < 8; y++) {
    for (int x = 1; x < 8; x++) {
      piece piece = board[calculatePosition(x, y)];
      int materialValue = getPieceValue(piece);

      if (!isWhitePiece(piece)) {
        value -= materialValue;
      } else {
        value += materialValue;
      }
    }
  }
  return value;
}

int calculatePosition(int x, int y) { return (y - 1) * 8 + x; }
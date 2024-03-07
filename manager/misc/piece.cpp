//
// Created by Marvin Becker on 05.03.24.
//

#include "./piece.h"

std::map<piece, char> pieceToCharMap = {
    {WP, 'P'}, {WN, 'N'}, {WB, 'B'},   {WR, 'R'}, {WQ, 'Q'},
    {WK, 'K'}, {BP, 'p'}, {BN, 'n'},   {BB, 'b'}, {BR, 'r'},
    {BQ, 'q'}, {BK, 'k'}, {EMPTY, ' '}
};

piece findKeyByValue(char value) {
  for (auto& pair : pieceToCharMap) {
    if (pair.second == value) return pair.first;
  }
  return EMPTY;
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

int calculatePosition(int x, int y) { return (y - 1) * 8 + x; }
//
// Created by Marvin Becker on 05.03.24.
//

#include "./piece.h"

int Piece::pieceValue[6] = {100, 300, 350, 500, 900, 1000};
char Piece::pieceToChar[13] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' '};

PieceType Piece::findKeyByValue(char value) {
  for (int i=0; i<13; ++i) {
    if (pieceToChar[i] == value) return static_cast<PieceType>(i);
  }
  return EMPTY;
}
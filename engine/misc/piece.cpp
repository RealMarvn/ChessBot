//
// Created by Marvin Becker on 05.03.24.
//

#include "./piece.h"

PieceType Piece::findKeyByValue(char value) {
  for (int i=0; i<13; ++i) {
    if (pieceToChar[i] == value) return static_cast<PieceType>(i);
  }
  return EMPTY;
}
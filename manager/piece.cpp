//
// Created by Marvin Becker on 05.03.24.
//

#include "./piece.h"

piece findKeyByValue(char value) {
  for (auto& pair : pieceToCharMap) {
    if (pair.second == value) return pair.first;
  }

  return EMPTY;
}
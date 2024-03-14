//
// Created by Marvin Becker on 05.03.24.
//

#include "./piece.h"

int Piece::pieceValue[6] = {100, 300, 350, 500, 900, 1000};
char Piece::pieceToChar[13] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k', ' '};

std::map<PieceType, char> pieceToCharMap = {{WP, 'P'}, {WN, 'N'}, {WB, 'B'},   {WR, 'R'}, {WQ, 'Q'},
                                            {WK, 'K'}, {BP, 'p'}, {BN, 'n'},   {BB, 'b'}, {BR, 'r'},
                                            {BQ, 'q'}, {BK, 'k'}, {EMPTY, ' '}};

Piece findKeyByValue(char value) {
  for (auto& pair : pieceToCharMap) {
    if (pair.second == value) return Piece(pair.first);
  }
  return Piece(EMPTY);
}

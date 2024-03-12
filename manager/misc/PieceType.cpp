//
// Created by Marvin Becker on 05.03.24.
//

#include "./PieceType.h"

Piece::Piece(PieceType piece) : pieceType(piece) {}

Piece::Piece() : pieceType(EMPTY) {}

bool Piece::isWhite() const { return (pieceType < BP); }

int Piece::getMaterialValue() const {
  if (pieceType == EMPTY) return 0;
  return ((isWhite() ? 1 : -1) * pieceValue[pieceType]);
}

char Piece::toChar() const { return pieceToCharMap[pieceType]; }

int Piece::pieceValue[6] = {100, 300, 350, 500, 900, 1000};
//WILL BE REMOVED
std::map<PieceType, char> Piece::pieceToCharMap = {
    {WP, 'P'}, {WN, 'N'}, {WB, 'B'},   {WR, 'R'}, {WQ, 'Q'},
    {WK, 'K'}, {BP, 'p'}, {BN, 'n'},   {BB, 'b'}, {BR, 'r'},
    {BQ, 'q'}, {BK, 'k'}, {EMPTY, ' '}};

std::map<PieceType, char> pieceToCharMap = {
    {WP, 'P'}, {WN, 'N'}, {WB, 'B'},   {WR, 'R'}, {WQ, 'Q'},
    {WK, 'K'}, {BP, 'p'}, {BN, 'n'},   {BB, 'b'}, {BR, 'r'},
    {BQ, 'q'}, {BK, 'k'}, {EMPTY, ' '}};

Piece findKeyByValue(char value) {
  for (auto& pair : pieceToCharMap) {
    if (pair.second == value) return Piece(pair.first);
  }
  return Piece(EMPTY);
}

int calculatePosition(int x, int y) { return ((y - 1) * 8 + x) - 1; }
//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <map>

enum piece { WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK, EMPTY = 14 };

extern std::map<piece, char> pieceToCharMap;

piece findKeyByValue(char value);

bool isWhitePiece(piece piece);

int calculatePosition(int x, int y);
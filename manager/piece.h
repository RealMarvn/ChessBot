//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <map>

enum piece { WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK, EMPTY = 14 };

std::map<piece, char> pieceToCharMap = {
    {WP, 'P'}, {WN, 'N'}, {WB, 'B'},   {WR, 'R'}, {WQ, 'Q'},
    {WK, 'K'}, {BP, 'p'}, {BN, 'n'},   {BB, 'b'}, {BR, 'r'},
    {BQ, 'q'}, {BK, 'k'}, {EMPTY, ' '}};

piece findKeyByValue(char value);
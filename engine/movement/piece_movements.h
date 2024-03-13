//
// Created by Marvin Becker on 07.03.24.
//

#pragma once
#include "../misc/PieceType.h"
#include "../misc/move.h"

bool canRookMove(int x, int y, int move_x, int move_y);
bool canQueenMove(int x, int y, int move_x, int move_y);
bool canPawnMove(int x, int y, int move_x, int move_y, bool capture,
                 bool isWhite);
bool canPawnPromote(bool isWhite, char figure, char promotion_figure,
                    int movePosition);
bool canKnightMove(int x, int y, int move_x, int move_y);
bool canKingMove(int x, int y, int move_x, int move_y);
bool canBishopMove(int x, int y, int move_x, int move_y);

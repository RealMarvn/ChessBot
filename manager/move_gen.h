//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <array>

static bool canRookMove(int x, int y, int move_x, int move_y);
static bool canQueenMove(int x, int y, int move_x, int move_y);
static bool canPawnMove(int x, int y, int move_x, int move_y, bool capture,
                        bool isWhite);
static bool canPawnPromote(bool isWhite, char figure, char promotion_figure,
                           int movePosition);
static bool canKnightMove(int x, int y, int move_x, int move_y);
static bool canKingMove(int x, int y, int move_x, int move_y);
static bool canBishopMove(int x, int y, int move_x, int move_y);
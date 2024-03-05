//
// Created by Marvin Becker on 10.02.24.
//

#pragma once

#include "../manager/board_manager.h"

std::pair<int, Move> miniMaxWithAlphaBetaCut(piece board[65], int depth, int alpha, int beta, bool isMaximisingPlayer);

int getPieceValue(piece piece);

int evaluateMaterialValue(piece board[65]);


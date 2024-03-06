//
// Created by Marvin Becker on 10.02.24.
//

#pragma once

#include "../manager/board_manager.h"

std::pair<int, Move> miniMaxWithAlphaBetaCut(std::array<piece, 65> board, int depth, int alpha, int beta, bool isMaximisingPlayer);

int getPieceValue(piece piece);

int evaluateMaterialValue(std::array<piece, 65> board);


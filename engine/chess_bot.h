//
// Created by Marvin Becker on 10.02.24.
//

#pragma once

#include ".././manager/move_gen.h"

extern int pieceValue[6];

int search(Board& boardManager, int depth, int ply, Move& bestMove);

int eval(Board& boardManager);

Move searchBestNextMove(Board& boardManager, int depth);
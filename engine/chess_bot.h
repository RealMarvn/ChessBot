//
// Created by Marvin Becker on 10.02.24.
//

#pragma once
#include "./movement/move_gen.h"

class ChessBot {
 public:
  Move searchBestNextMove(Board& board, int depth);

 private:
  int search(Board& boardManager, int depth, int alpha, int beta, int ply, Move& bestMove);
  int eval(Board& board);
};

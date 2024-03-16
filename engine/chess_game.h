//
// Created by Marvin Becker on 16.03.24.
//

#pragma once

#include <memory>

#include "./chess_bot.h"

class ChessGame {
 public:
  ChessGame() : board{new Board}, bot{new ChessBot} {};
  void start();

 private:
  std::unique_ptr<Board> board;
  std::unique_ptr<ChessBot> bot;
};

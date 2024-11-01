//
// Created by Marvin Becker on 14.03.24.
//

#pragma once

struct board_setting {
  // EP square
  int epSquare{100};
  // Castling moves.
  bool whiteQueenSide{false};
  bool whiteKingSide{false};
  bool blackQueenSide{false};
  bool blackKingSide{false};

  // For FEN
  int lastMovesSincePawnOrCapture{0};
  int turns{1};
};

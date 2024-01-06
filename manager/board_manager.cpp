//
// Created by Marvin Becker on 15.12.23.
//

#include "board_manager.h"

#include <iostream>

BoardManager::BoardManager() {
  player = WHITE;
  Piece piece{};
  piece.figure = ' ';
  piece.moved = false;
  board[0] = piece;

  // Initalize left side
  piece.figure = 'R';
  board[1] = piece;
  piece.figure = 'N';
  board[2] = piece;
  piece.figure = 'B';
  board[3] = piece;

  piece.figure = 'Q';
  board[4] = piece;
  piece.figure = 'K';
  board[5] = piece;

  piece.figure = 'B';
  board[6] = piece;
  piece.figure = 'N';
  board[7] = piece;
  piece.figure = 'R';
  board[8] = piece;

  piece.figure = 'P';
  for (int i = 9; i <= 16; i++) {
    board[i] = piece;
  }

  piece.figure = ' ';
  for (int i = 17; i <= 49; i++) {
    board[i] = piece;
  }

  piece.figure = 'p';
  for (int i = 49; i <= 56; i++) {
    board[i] = piece;
  }

  piece.figure = 'r';
  board[57] = piece;
  piece.figure = 'n';
  board[58] = piece;
  piece.figure = 'b';
  board[59] = piece;

  piece.figure = 'q';
  board[60] = piece;
  piece.figure = 'k';
  board[61] = piece;

  piece.figure = 'b';
  board[62] = piece;
  piece.figure = 'n';
  board[63] = piece;
  piece.figure = 'r';
  board[64] = piece;
}

bool BoardManager::popLastMove() {
  if (moves.empty()) {
    return false;
  }
  Move last_move = moves.back();
  moves.pop_back();
  board[last_move.position] = last_move.capturedFigure;
  board[last_move.old_position] = last_move.figure;
  player = player == WHITE ? BLACK : WHITE;
  return true;
}

void BoardManager::saveMove(int movePosition, int position) {
  Move move{};
  move.capturedFigure = board[movePosition];
  move.figure = board[position];
  move.position = movePosition;
  move.old_position = position;
  moves.push_back(move);
}

bool BoardManager::movePiece(char fig, int x, int y, int move_x, int move_y,
                             bool capture, char promotion_figure) {

  if (!canMove(fig, x, y, move_x, move_y, capture)) {
    std::cout << "invalid" << std::endl;
    return false;
  }

  bool promotion = false;
  int position = calculatePosition(x, y);
  int movePosition = calculatePosition(move_x, move_y);

  if (promotion_figure != ' ') {
    if (!canPawnPromote(player == WHITE, promotion_figure, movePosition)) {
      std::cout << "invalid" << std::endl;
      return false;
    }
    promotion = true;
  }

  saveMove(movePosition, position);

  board[position].moved = true;
  board[movePosition] = board[position];
  board[position].figure = ' ';
  player = player == WHITE ? BLACK : WHITE;

  if (promotion) {
    board[movePosition].figure = promotion_figure;
  }

  if (isKingInDanger(false)) {
    std::cout << "yes" << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }
  return true;
}

bool BoardManager::canMove(char fig, int x, int y, int move_x, int move_y,
                           bool capture) {

  int position = calculatePosition(x, y);
  int move = calculatePosition(move_x, move_y);

  // Check if position is out of bounds!
  if (position < 1 || position > 64 || move < 1 || move > 64) {
    return false;
  }

  // Check if figure is really that figure.
  if (board[position].figure != fig) {
    return false;
  }

  // Check if capture an empty field or a field without a capture
  if ((board[move].figure != ' ' && !capture) ||
      (capture && board[move].figure == ' ')) {
    return false;
  }

  if (capture) {
    // Check if you try to capture your own team.
    if ((isupper(board[position].figure) && isupper(board[move].figure)) ||
        (islower(board[position].figure) && islower(board[move].figure))) {
      return false;
    }
  }

  switch (tolower(fig)) {
  case 'p':
    if (isPathClear(x, y, move_x, move_y, board)) {
      return canPawnMove(x, y, move_x, move_y, capture,
                         board[calculatePosition(x, y)].moved, player == WHITE);
    }
    break;
  case 'k':
    if (isPathClear(x, y, move_x, move_y, board)) {
      return canKingMove(x, y, move_x, move_y);
    }
    break;
  case 'b':
    if (isPathClear(x, y, move_x, move_y, board)) {
      return canBishopMove(x, y, move_x, move_y);
    }
    break;
  case 'r':
    if (isPathClear(x, y, move_x, move_y, board)) {
      return canRookMove(x, y, move_x, move_y);
    }
  case 'q':
    if (isPathClear(x, y, move_x, move_y, board)) {
      return canQueenMove(x, y, move_x, move_y);
    }
  case 'n':
    return canKnightMove(x, y, move_x, move_y);
  }

  return false;
}

bool BoardManager::isPathClear(int startX, int startY, int endX, int endY,
                               Piece board[65]) {
  int dx = std::abs(endX - startX);
  int dy = std::abs(endY - startY);
  int x = startX;
  int y = startY;
  int xInc = (endX > startX) ? 1 : -1;
  int yInc = (endY > startY) ? 1 : -1;

  if (dx > dy) {
    int p = 2 * dy - dx;
    while (x != endX) {
      if (p >= 0) {
        y += yInc;
        p -= 2 * dx;
      }
      x += xInc;
      p += 2 * dy;

      if (x != endX || y != endY) {
        if (board[calculatePosition(x, y)].figure != ' ') {
          return false;
        }
      }
    }
  } else {
    int p = 2 * dx - dy;
    while (y != endY) {
      if (p >= 0) {
        x += xInc;
        p -= 2 * dy;
      }
      y += yInc;
      p += 2 * dx;

      if (x != endX || y != endY) {
        if (board[calculatePosition(x, y)].figure != ' ') {
          return false;
        }
      }
    }
  }

  return true; // Der Pfad ist klar
}

bool BoardManager::isKingInDanger(bool justReadIn) {
  int whiteKingPositionX = 0;
  int whiteKingPositionY = 0;
  int blackKingPositionX = 0;
  int blackKingPositionY = 0;

  // Get King position
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      char figure = board[calculatePosition(x, y)].figure;
      if (figure == 'k') {
        blackKingPositionX = x;
        blackKingPositionY = y;
      } else if (figure == 'K') {
        whiteKingPositionX = x;
        whiteKingPositionY = y;
      }
    }
  }

  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      char figure = board[calculatePosition(x, y)].figure;
      if (islower(figure)) {
        if (figure == 'r') {
        }
        if (canMove(figure, x, y, whiteKingPositionX, whiteKingPositionY,
                    true)) {
          return true;
        }
      } else if (isupper(figure) && !justReadIn) {
        if (canMove(figure, x, y, blackKingPositionX, blackKingPositionY,
                    true)) {
          return true;
        }
      }
    }
  }
  return false;
}

int BoardManager::calculatePosition(int x, int y) { return (y - 1) * 8 + x; }

void BoardManager::readInBoard(std::string input) {
  if (input.length() == 64) {
    player = WHITE;

    int inputIndex = 0;
    for (int y = 8; y > 0; y--) {
      for (int x = 1; x < 9; x++) {
        board[calculatePosition(x, y)].figure = input[inputIndex];
        inputIndex++;
      }
    }

    if (isKingInDanger(true)) {
      std::cout << "yes" << std::endl;
    } else {
      std::cout << "no" << std::endl;
    }
  }
}

void BoardManager::printCurrentBoard(bool debug) {
  if (debug) {
    return;
  }

  if (player == WHITE) {
    std::cout << "Current turn: "
              << "White" << std::endl;
  } else {
    std::cout << "Current turn: "
              << "Black" << std::endl;
  }

  for (int y = 8; y >= 1; y--) {
    std::cout << y << " | ";
    for (int x = 1; x <= 8; x++) {
      std::cout << "[" << board[calculatePosition(x, y)].figure << "]";
    }
    std::cout << std::endl;
  }
  std::cout << "     a";
  for (int i = 2; i <= 8; i++) {
    std::cout << "  " << char(i + 96);
  }
  std::cout << std::endl;
}

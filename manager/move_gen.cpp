//
// Created by Marvin Becker on 05.03.24.
//
#include "./move_gen.h"

// std::array<Move, 27> getPossiblePieceMoves(int x, int y, std::array<piece,
// 65> board) {
//   std::array<Move, 27> possibleMoves;
//   return possibleMoves;
// }

std::vector<Move> getAllPossibleRookMoves(std::pair<int, int> startPos,
                                          piece board[65]) {
  int old_position = calculatePosition(startPos.first, startPos.second);
  bool isWhite = isWhitePiece(board[old_position]);
  std::vector<Move> possibleMoves{};

  Move move{};
  move.old_position = old_position;
  move.figure = isWhite ? WR : BR;

  std::pair<int, int> directions[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  for (const auto& dir : directions) {
    int x = startPos.first + dir.first;
    int y = startPos.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.position = position;
      if (board[position] == EMPTY) {
        move.capturedFigure = EMPTY;
        possibleMoves.push_back(move);
      } else if (board[position] != EMPTY) {
        if ((isWhite && isWhitePiece(board[position])) ||
            (!isWhite && !isWhitePiece(board[position]))) {
          break;
        }
        move.capturedFigure = board[position];
        possibleMoves.push_back(move);
        break;
      } else {
        break;
      }

      x += dir.first;
      y += dir.second;
    }
  }
  return possibleMoves;
}

// ################### PIECE MOVES ###################

bool canRookMove(int x, int y, int move_x, int move_y) {
  if (x == move_x && y != move_y) return true;
  if (y == move_y && x != move_x) return true;

  return false;
}

bool canQueenMove(int x, int y, int move_x, int move_y) {
  if (x == move_x && y != move_y) return true;
  if (y == move_y && x != move_x) return true;
  if (std::abs(y - move_y) == std::abs(x - move_x)) return true;
  return false;
}

bool canPawnMove(int x, int y, int move_x, int move_y, bool capture,
                 bool isWhite) {
  int rowDiff = std::abs(move_y - y);
  int colDiff = std::abs(move_x - x);

  if (rowDiff > 0) {
    if ((isWhite && move_y < y) || (!isWhite && move_y > y)) {
      return false;
    }

    if (colDiff == 0 && rowDiff == 1 && !capture) {
      return true;
    }

    if (colDiff == 0 && rowDiff == 2 && !capture) {
      if (isWhite && y == 2 || !isWhite && y == 7) {
        return true;
      }
    }

    if (colDiff == 1 && rowDiff == 1 && capture) {
      return true;
    }
  }

  return false;
}

bool canPawnPromote(bool isWhite, char fig, char promotion_figure,
                    int movePosition) {
  if (tolower(fig) == 'p') {
    if ((movePosition >= 56 && movePosition <= 64) ||
        (movePosition >= 1 && movePosition <= 8)) {
      if ((isWhite && isupper(promotion_figure)) ||
          (!isWhite && islower(promotion_figure))) {
        switch (tolower(promotion_figure)) {
          case 'r':
          case 'n':
          case 'q':
          case 'b':
            return true;
        }
      }
    }
  }
  return false;
}

bool canKnightMove(int x, int y, int move_x, int move_y) {
  int rowDiff = std::abs(y - move_y);
  int colDiff = std::abs(x - move_x);

  return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}

bool canKingMove(int x, int y, int move_x, int move_y) {
  if ((y == move_y && abs(x - move_x) == 1)) return true;
  if ((x == move_x && abs(y - move_y) == 1)) return true;
  if (std::abs(y - move_y) == std::abs(x - move_x) && std::abs(y - move_y) == 1)
    return true;

  return false;
}

bool canBishopMove(int x, int y, int move_x, int move_y) {
  return std::abs(y - move_y) == std::abs(x - move_x);
}
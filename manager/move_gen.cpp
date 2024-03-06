//
// Created by Marvin Becker on 05.03.24.
//
#include "./move_gen.h"

AllPseudoMoves getAllPseudoLegalMoves(std::array<piece, 65>& board,
                                      bool player) {
  AllPseudoMoves allPseudoMoves;
  for (int y = 1; y < 9; y++) {
    for (int x = 1; x < 9; x++) {
      piece piece = board[calculatePosition(x, y)];
      if (piece != EMPTY) {
        if ((isWhitePiece(piece) && player) ||
            (!isWhitePiece(piece) && !player)) {
          switch (piece) {
            case BP:
            case WP:
              getAllPossiblePawnMoves({x, y}, board, allPseudoMoves);
              break;
            case BN:
            case WN:
              getAllPossibleKnightMoves({x, y}, board, allPseudoMoves);
              break;
            case BB:
            case WB:
              getAllPossibleBishopMoves({x, y}, board, allPseudoMoves);
              break;
            case BR:
            case WR:
              getAllPossibleRookMoves({x, y}, board, allPseudoMoves);
              break;
            case BQ:
            case WQ:
              getAllPossibleQueenMoves({x, y}, board, allPseudoMoves);
              break;
            case BK:
            case WK:
              getAllPossibleKingMoves({x, y}, board, allPseudoMoves);
              break;
            case EMPTY:
              break;
          }
        }
      }
    }
  }
  return allPseudoMoves;
}

bool isKingInCheck(bool pieceColor, std::array<piece, 65>& board) {
  int kingSquareX = 0;
  int kingSquareY = 0;
  AllPseudoMoves allKnightMoves;
  AllPseudoMoves allPawnMoves;
  AllPseudoMoves allBishopMoves;
  AllPseudoMoves allRookMoves;

  // Get King moved_position
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      if (board[calculatePosition(x, y)] == (pieceColor ? WK : BK)) {
        kingSquareX = x;
        kingSquareY = y;
        break;
      }
    }
  }
  getAllPossibleKnightMoves({kingSquareX, kingSquareY}, board, allKnightMoves);
  for (int i = 0; i < allKnightMoves.index; i++) {
    if (allKnightMoves.move_list[i].capturedPiece == WN ||
        allKnightMoves.move_list[i].capturedPiece == BN) {
      return true;
    }
  }

  //  for (const Move& move :
  //       getAllPossibleKnightMoves({kingSquareX, kingSquareY}, board)) {
  //    if (move.capturedPiece == WK || move.capturedPiece == BK) {
  //      return true;
  //    }
  //  }

  getAllPossiblePawnMoves({kingSquareX, kingSquareY}, board, allPawnMoves);
  for (int i = 0; i < allPawnMoves.index; i++) {
    if (allPawnMoves.move_list[i].capturedPiece == WP ||
        allPawnMoves.move_list[i].capturedPiece == BP) {
      return true;
    }
  }

  //  for (const Move& move :
  //       getAllPossiblePawnMoves({kingSquareX, kingSquareY}, board)) {
  //    if (move.capturedPiece == WP || move.capturedPiece == BP) {
  //      return true;
  //    }
  //  }

  getAllPossibleBishopMoves({kingSquareX, kingSquareY}, board, allBishopMoves);
  for (int i = 0; i < allBishopMoves.index; i++) {
    if (allBishopMoves.move_list[i].capturedPiece == WB ||
        allBishopMoves.move_list[i].capturedPiece == BB ||
        allBishopMoves.move_list[i].capturedPiece == WQ ||
        allBishopMoves.move_list[i].capturedPiece == BQ) {
      return true;
    }
  }

  //  for (const Move& move :
  //       getAllPossibleBishopMoves({kingSquareX, kingSquareY}, board)) {
  //    if (move.capturedPiece == WB || move.capturedPiece == BB ||
  //        move.capturedPiece == WQ || move.capturedPiece == BQ) {
  //      return true;
  //    }
  //  }

  getAllPossibleRookMoves({kingSquareX, kingSquareY}, board, allRookMoves);
  for (int i = 0; i < allBishopMoves.index; i++) {
    if (allRookMoves.move_list[i].capturedPiece == WR ||
        allRookMoves.move_list[i].capturedPiece == BR ||
        allRookMoves.move_list[i].capturedPiece == WQ ||
        allRookMoves.move_list[i].capturedPiece == BQ) {
      return true;
    }
  }

  //  for (const Move& move :
  //       getAllPossibleRookMoves({kingSquareX, kingSquareY}, board)) {
  //    if (move.capturedPiece == WR || move.capturedPiece == BR ||
  //        move.capturedPiece == WQ || move.capturedPiece == BQ) {
  //      return true;
  //    }
  //  }

  return false;
}

void getAllPossibleRookMoves(std::pair<int, int> startSquare,
                             std::array<piece, 65>& board,
                             AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(board[old_position]);

  Move move{};
  move.original_position = old_position;
  move.movedPiece = pieceColor ? WR : BR;

  std::pair<int, int> directions[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.moved_position = position;
      move.capturedPiece = board[position];

      if (board[position] == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (board[position] != EMPTY) {
        if ((pieceColor && isWhitePiece(board[position])) ||
            (!pieceColor && !isWhitePiece(board[position]))) {
          break;
        }
        allPseudoMoves.push_back(move);
        break;
      } else {
        break;
      }

      x += dir.first;
      y += dir.second;
    }
  }
}

void getAllPossibleBishopMoves(std::pair<int, int> startSquare,
                               std::array<piece, 65>& board,
                               AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(board[old_position]);

  Move move{};
  move.original_position = old_position;
  move.movedPiece = pieceColor ? WB : BB;

  std::pair<int, int> directions[4] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.moved_position = position;
      move.capturedPiece = board[position];

      if (board[position] == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (board[position] != EMPTY) {
        if ((pieceColor && isWhitePiece(board[position])) ||
            (!pieceColor && !isWhitePiece(board[position]))) {
          break;
        }
        allPseudoMoves.push_back(move);
        break;
      } else {
        break;
      }

      x += dir.first;
      y += dir.second;
    }
  }
}

void getAllPossibleQueenMoves(std::pair<int, int> startSquare,
                              std::array<piece, 65>& board,
                              AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(board[old_position]);

  Move move{};
  move.original_position = old_position;
  move.movedPiece = pieceColor ? WQ : BQ;

  std::pair<int, int> directions[8] = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                                       {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.moved_position = position;
      move.capturedPiece = board[position];

      if (board[position] == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (board[position] != EMPTY) {
        if ((pieceColor && isWhitePiece(board[position])) ||
            (!pieceColor && !isWhitePiece(board[position]))) {
          break;
        }
        allPseudoMoves.push_back(move);
        break;
      } else {
        break;
      }

      x += dir.first;
      y += dir.second;
    }
  }
}

void getAllPossibleKingMoves(std::pair<int, int> startSquare,
                             std::array<piece, 65>& board,
                             AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(board[old_position]);

  Move move{};
  move.original_position = old_position;
  move.movedPiece = pieceColor ? WK : BK;

  std::pair<int, int> directions[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                       {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;
    int position = calculatePosition(x, y);

    piece piece = board[calculatePosition(x, y)];
    if (x > 0 && y > 0 && x < 9 && y < 9) {
      if (piece != EMPTY) {
        if ((pieceColor && isWhitePiece(piece)) ||
            ((!pieceColor) && (!isWhitePiece(piece)))) {
          continue;
        }
      }
      move.moved_position = position;
      move.capturedPiece = board[position];
      allPseudoMoves.push_back(move);
    }
  }
}

void getAllPossibleKnightMoves(std::pair<int, int> startSquare,
                               std::array<piece, 65>& board,
                               AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(board[old_position]);

  Move move{};
  move.original_position = old_position;
  move.movedPiece = pieceColor ? WN : BN;

  std::pair<int, int> directions[8] = {{-2, -1}, {-1, -2}, {1, -2}, {2, -1},
                                       {2, 1},   {1, 2},   {-1, 2}, {-2, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;
    int position = calculatePosition(x, y);

    piece piece = board[calculatePosition(x, y)];
    if (x > 0 && y > 0 && x < 9 && y < 9) {
      if (piece != EMPTY) {
        if ((pieceColor && isWhitePiece(piece)) ||
            ((!pieceColor) && (!isWhitePiece(piece)))) {
          continue;
        }
      }
      move.moved_position = position;
      move.capturedPiece = board[position];
      allPseudoMoves.push_back(move);
    }
  }
}

void getAllPossiblePawnMoves(std::pair<int, int> startSquare,
                             std::array<piece, 65>& board,
                             AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(board[old_position]);

  Move move{};
  move.original_position = old_position;
  move.movedPiece = pieceColor ? WP : BP;

  std::pair<int, int> directions[4] = {{0, 1}, {-1, 1}, {1, 1}, {0, 2}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    if (!pieceColor) {
      x = startSquare.first - dir.first;
      y = startSquare.second - dir.second;
    }

    int position = calculatePosition(x, y);

    piece piece = board[calculatePosition(x, y)];
    if (x > 0 && y > 0 && x < 9 && y < 9) {
      if (piece != EMPTY && dir.first != 0 && dir.second != 0) {
        if ((pieceColor && isWhitePiece(piece)) ||
            ((!pieceColor) && (!isWhitePiece(piece)))) {
          continue;
        }
        move.moved_position = position;
        move.capturedPiece = board[position];
        allPseudoMoves.push_back(move);
      } else if (dir.first == 0 && dir.second != 0 && piece == EMPTY) {
        if (dir.second == 2) {
          if ((pieceColor && startSquare.second == 2) || (!pieceColor && startSquare.second == 7)) {
            if (board[calculatePosition(x, startSquare.second + (pieceColor ? 1 : -1))] == EMPTY) {
              move.moved_position = position;
              move.capturedPiece = board[position];
              allPseudoMoves.push_back(move);
            }
            }
            continue;
        }
        move.moved_position = position;
        move.capturedPiece = board[position];
        allPseudoMoves.push_back(move);
      }
    }
  }
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
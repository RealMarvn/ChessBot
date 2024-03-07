//
// Created by Marvin Becker on 05.03.24.
//
#include "./move_gen.h"

AllPseudoMoves getAllPseudoLegalMoves(BoardManager& boardManager, bool player) {
  AllPseudoMoves allPseudoMoves;
  for (int y = 1; y < 9; y++) {
    for (int x = 1; x < 9; x++) {
      piece piece = boardManager.board[calculatePosition(x, y)];
      if (piece != EMPTY) {
        if ((isWhitePiece(piece) && player) ||
            (!isWhitePiece(piece) && !player)) {
          switch (piece) {
            case BP:
            case WP:
              getAllPossiblePawnMoves({x, y}, boardManager, allPseudoMoves);
              break;
            case BN:
            case WN:
              getAllPossibleKnightMoves({x, y}, boardManager, allPseudoMoves);
              break;
            case BB:
            case WB:
              getAllPossibleBishopMoves({x, y}, boardManager, allPseudoMoves);
              break;
            case BR:
            case WR:
              getAllPossibleRookMoves({x, y}, boardManager, allPseudoMoves);
              break;
            case BQ:
            case WQ:
              getAllPossibleQueenMoves({x, y}, boardManager, allPseudoMoves);
              break;
            case BK:
            case WK:
              getAllPossibleKingMoves({x, y}, boardManager, allPseudoMoves);
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

bool isKingInCheck(bool pieceColor, BoardManager& boardManager) {
  int kingSquareX = 0;
  int kingSquareY = 0;
  AllPseudoMoves allKnightMoves;
  AllPseudoMoves allPawnMoves;
  AllPseudoMoves allBishopMoves;
  AllPseudoMoves allRookMoves;

  // Get King moveSquare
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      if (boardManager.board[calculatePosition(x, y)] ==
          (pieceColor ? WK : BK)) {
        kingSquareX = x;
        kingSquareY = y;
        break;
      }
    }
  }

  getAllPossibleKnightMoves({kingSquareX, kingSquareY}, boardManager,
                            allKnightMoves);
  for (int i = 0; i < allKnightMoves.index; i++) {
    if (allKnightMoves.move_list[i].capturedPiece == WN ||
        allKnightMoves.move_list[i].capturedPiece == BN) {
      return true;
    }
  }

  getAllPossiblePawnMoves({kingSquareX, kingSquareY}, boardManager,
                          allPawnMoves);
  for (int i = 0; i < allPawnMoves.index; i++) {
    if (allPawnMoves.move_list[i].capturedPiece == WP ||
        allPawnMoves.move_list[i].capturedPiece == BP) {
      return true;
    }
  }

  getAllPossibleBishopMoves({kingSquareX, kingSquareY}, boardManager,
                            allBishopMoves);
  for (int i = 0; i < allBishopMoves.index; i++) {
    if (allBishopMoves.move_list[i].capturedPiece == WB ||
        allBishopMoves.move_list[i].capturedPiece == BB ||
        allBishopMoves.move_list[i].capturedPiece == WQ ||
        allBishopMoves.move_list[i].capturedPiece == BQ) {
      return true;
    }
  }

  getAllPossibleRookMoves({kingSquareX, kingSquareY}, boardManager,
                          allRookMoves);
  for (int i = 0; i < allRookMoves.index; i++) {
    if (allRookMoves.move_list[i].capturedPiece == WR ||
        allRookMoves.move_list[i].capturedPiece == BR ||
        allRookMoves.move_list[i].capturedPiece == WQ ||
        allRookMoves.move_list[i].capturedPiece == BQ) {
      return true;
    }
  }

  return false;
}

void getAllPossibleRookMoves(std::pair<int, int> startSquare,
                             BoardManager& boardManager,
                             AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(boardManager.board[old_position]);

  Move move{};
  move.square = old_position;
  move.movingPiece = pieceColor ? WR : BR;

  std::pair<int, int> directions[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.moveSquare = position;
      move.capturedPiece = boardManager.board[position];

      if (boardManager.board[position] == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (boardManager.board[position] != EMPTY) {
        if ((pieceColor && isWhitePiece(boardManager.board[position])) ||
            (!pieceColor && !isWhitePiece(boardManager.board[position]))) {
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
                               BoardManager& boardManager,
                               AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(boardManager.board[old_position]);

  Move move{};
  move.square = old_position;
  move.movingPiece = pieceColor ? WB : BB;

  std::pair<int, int> directions[4] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.moveSquare = position;
      move.capturedPiece = boardManager.board[position];

      if (boardManager.board[position] == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (boardManager.board[position] != EMPTY) {
        if ((pieceColor && isWhitePiece(boardManager.board[position])) ||
            (!pieceColor && !isWhitePiece(boardManager.board[position]))) {
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
                              BoardManager& boardManager,
                              AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(boardManager.board[old_position]);

  Move move{};
  move.square = old_position;
  move.movingPiece = pieceColor ? WQ : BQ;

  std::pair<int, int> directions[8] = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                                       {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.moveSquare = position;
      move.capturedPiece = boardManager.board[position];

      if (boardManager.board[position] == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (boardManager.board[position] != EMPTY) {
        if ((pieceColor && isWhitePiece(boardManager.board[position])) ||
            (!pieceColor && !isWhitePiece(boardManager.board[position]))) {
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
                             BoardManager& boardManager,
                             AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(boardManager.board[old_position]);

  Move move{};
  move.square = old_position;
  move.movingPiece = pieceColor ? WK : BK;

  std::pair<int, int> directions[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                       {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;
    int position = calculatePosition(x, y);

    piece piece = boardManager.board[calculatePosition(x, y)];
    if (x > 0 && y > 0 && x < 9 && y < 9) {
      if (piece != EMPTY) {
        if ((pieceColor && isWhitePiece(piece)) ||
            ((!pieceColor) && (!isWhitePiece(piece)))) {
          continue;
        }
      }
      move.moveSquare = position;
      move.capturedPiece = boardManager.board[position];
      allPseudoMoves.push_back(move);
    }
  }
}

void getAllPossibleKnightMoves(std::pair<int, int> startSquare,
                               BoardManager& boardManager,
                               AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(boardManager.board[old_position]);

  Move move{};
  move.square = old_position;
  move.movingPiece = pieceColor ? WN : BN;

  std::pair<int, int> directions[8] = {{-2, -1}, {-1, -2}, {1, -2}, {2, -1},
                                       {2, 1},   {1, 2},   {-1, 2}, {-2, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;
    int position = calculatePosition(x, y);

    piece piece = boardManager.board[calculatePosition(x, y)];
    if (x > 0 && y > 0 && x < 9 && y < 9) {
      if (piece != EMPTY) {
        if ((pieceColor && isWhitePiece(piece)) ||
            ((!pieceColor) && (!isWhitePiece(piece)))) {
          continue;
        }
      }
      move.moveSquare = position;
      move.capturedPiece = boardManager.board[position];
      allPseudoMoves.push_back(move);
    }
  }
}

void getAllPossiblePawnMoves(std::pair<int, int> startSquare,
                             BoardManager& boardManager,
                             AllPseudoMoves& allPseudoMoves) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);
  bool pieceColor = isWhitePiece(boardManager.board[old_position]);

  std::pair<int, int> directions[4] = {{0, 1}, {-1, 1}, {1, 1}, {0, 2}};

  for (const auto& dir : directions) {
    Move move{};
    move.square = old_position;
    move.movingPiece = pieceColor ? WP : BP;

    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    if (!pieceColor) {
      x = startSquare.first - dir.first;
      y = startSquare.second - dir.second;
    }

    int position = calculatePosition(x, y);

    piece piece = boardManager.board[calculatePosition(x, y)];

    if (x > 0 && y > 0 && x < 9 && y < 9) {
      if (dir.first != 0 && dir.second != 0) {
        if (piece != EMPTY) {
          if ((pieceColor && isWhitePiece(piece)) ||
              ((!pieceColor) && (!isWhitePiece(piece)))) {
            continue;
          }
          if ((pieceColor && y == 8) || (!pieceColor && y == 1)) {
            move.moveType = PROMOTION;
            for (int promotionIndex = 0; promotionIndex < 4; promotionIndex++) {
              move.promotionPiece =
                  (pieceColor ? whitePawnPossiblePromotions[promotionIndex]
                              : blackPawnPossiblePromotions[promotionIndex]);
              move.moveSquare = position;
              move.capturedPiece = boardManager.board[position];
              allPseudoMoves.push_back(move);
            }
            continue;
          }

          move.moveSquare = position;
          move.capturedPiece = boardManager.board[position];
          allPseudoMoves.push_back(move);
        } else {
          if (boardManager.epSquare != 100 &&
              calculatePosition(x, y) == boardManager.epSquare) {
            move.moveType = EN_PASSANT;
            move.moveSquare = position;
            move.capturedPiece = boardManager.board[position];
            allPseudoMoves.push_back(move);
          }
        }
      } else if (dir.first == 0 && dir.second != 0 && piece == EMPTY) {
        if (dir.second == 2) {
          if ((pieceColor && startSquare.second == 2) ||
              (!pieceColor && startSquare.second == 7)) {
            if (boardManager.board[calculatePosition(
                    x, startSquare.second + (pieceColor ? 1 : -1))] == EMPTY) {
              move.moveSquare = position;
              move.capturedPiece = boardManager.board[position];
              allPseudoMoves.push_back(move);
            }
          }
          continue;
        }

        if ((pieceColor && y == 8) || (!pieceColor && y == 1)) {
          move.moveType = PROMOTION;
          for (int promotionIndex = 0; promotionIndex < 4; promotionIndex++) {
            move.promotionPiece =
                (pieceColor ? whitePawnPossiblePromotions[promotionIndex]
                            : blackPawnPossiblePromotions[promotionIndex]);
            move.moveSquare = position;
            move.capturedPiece = boardManager.board[position];
            allPseudoMoves.push_back(move);
          }
          continue;
        }

        move.moveSquare = position;
        move.capturedPiece = boardManager.board[position];
        allPseudoMoves.push_back(move);
      }
    }
  }
}
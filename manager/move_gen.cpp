//
// Created by Marvin Becker on 05.03.24.
//
#include "./move_gen.h"

AllPseudoMoves getAllPseudoLegalMoves(Board& boardManager, bool player) {
  AllPseudoMoves allPseudoMoves;
  for (int y = 1; y < 9; y++) {
    for (int x = 1; x < 9; x++) {
      Piece piece = boardManager[calculatePosition(x, y)];
      if (piece.pieceType != EMPTY) {
        if ((piece.isWhite() && player) ||
            (!piece.isWhite() && !player)) {
          switch (piece.pieceType) {
            case BP:
            case WP:
              getAllPossiblePawnMoves({x, y}, boardManager, allPseudoMoves,
                                      player);
              break;
            case BN:
            case WN:
              getAllPossibleKnightMoves({x, y}, boardManager, allPseudoMoves,
                                        player);
              break;
            case BB:
            case WB:
              getAllPossibleBishopMoves({x, y}, boardManager, allPseudoMoves,
                                        player);
              break;
            case BR:
            case WR:
              getAllPossibleRookMoves({x, y}, boardManager, allPseudoMoves,
                                      player);
              break;
            case BQ:
            case WQ:
              getAllPossibleQueenMoves({x, y}, boardManager, allPseudoMoves,
                                       player);
              break;
            case BK:
            case WK:
              getAllPossibleKingMoves({x, y}, boardManager, allPseudoMoves,
                                      player);
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

bool isKingInCheck(bool pieceColor, Board& boardManager) {
  // Get King moveSquare
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      if (boardManager[calculatePosition(x, y)].pieceType ==
          (pieceColor ? WK : BK)) {
        return isSquareAttacked({x, y}, boardManager, pieceColor);
      }
    }
  }

  return false;
}

bool isSquareAttacked(std::pair<int, int> square, Board& boardManager,
                      bool pieceColor) {
  AllPseudoMoves allKnightMoves;
  AllPseudoMoves allPawnMoves;
  AllPseudoMoves allBishopMoves;
  AllPseudoMoves allRookMoves;

  getAllPossibleKnightMoves(square, boardManager, allKnightMoves, pieceColor);
  for (int i = 0; i < allKnightMoves.index; i++) {
    if (allKnightMoves.move_list[i].capturedPiece.pieceType == WN ||
        allKnightMoves.move_list[i].capturedPiece.pieceType == BN) {
      return true;
    }
  }

  getAllPossiblePawnMoves(square, boardManager, allPawnMoves, pieceColor);
  for (int i = 0; i < allPawnMoves.index; i++) {
    if (allPawnMoves.move_list[i].capturedPiece.pieceType == WP ||
        allPawnMoves.move_list[i].capturedPiece.pieceType == BP) {
      return true;
    }
  }

  getAllPossibleBishopMoves(square, boardManager, allBishopMoves, pieceColor);
  for (int i = 0; i < allBishopMoves.index; i++) {
    if (allBishopMoves.move_list[i].capturedPiece.pieceType == WB ||
        allBishopMoves.move_list[i].capturedPiece.pieceType == BB ||
        allBishopMoves.move_list[i].capturedPiece.pieceType == WQ ||
        allBishopMoves.move_list[i].capturedPiece.pieceType == BQ) {
      return true;
    }
  }

  getAllPossibleRookMoves(square, boardManager, allRookMoves, pieceColor);
  for (int i = 0; i < allRookMoves.index; i++) {
    if (allRookMoves.move_list[i].capturedPiece.pieceType == WR ||
        allRookMoves.move_list[i].capturedPiece.pieceType == BR ||
        allRookMoves.move_list[i].capturedPiece.pieceType == WQ ||
        allRookMoves.move_list[i].capturedPiece.pieceType == BQ) {
      return true;
    }
  }

  return false;
}

void getAllPossibleRookMoves(std::pair<int, int> startSquare,
                             Board& boardManager,
                             AllPseudoMoves& allPseudoMoves, bool pieceColor) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = pieceColor ? WR : BR;

  std::pair<int, int> directions[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.moveSquare = position;
      move.capturedPiece = boardManager[position];

      if (boardManager[position].pieceType == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (boardManager[position].pieceType != EMPTY) {
        if ((pieceColor && boardManager[position].isWhite()) ||
            (!pieceColor && !boardManager[position].isWhite())) {
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
                               Board& boardManager,
                               AllPseudoMoves& allPseudoMoves,
                               bool pieceColor) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = pieceColor ? WB : BB;

  std::pair<int, int> directions[4] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.moveSquare = position;
      move.capturedPiece = boardManager[position];

      if (boardManager[position].pieceType == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (boardManager[position].pieceType != EMPTY) {
        if ((pieceColor && boardManager[position].isWhite()) ||
            (!pieceColor && !boardManager[position].isWhite())) {
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
                              Board& boardManager,
                              AllPseudoMoves& allPseudoMoves, bool pieceColor) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = pieceColor ? WQ : BQ;

  std::pair<int, int> directions[8] = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                                       {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.moveSquare = position;
      move.capturedPiece = boardManager[position];

      if (boardManager[position].pieceType == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (boardManager[position].pieceType != EMPTY) {
        if ((pieceColor && boardManager[position].isWhite()) ||
            (!pieceColor && !boardManager[position].isWhite())) {
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
                             Board& boardManager,
                             AllPseudoMoves& allPseudoMoves, bool pieceColor) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = pieceColor ? WK : BK;

  std::pair<int, int> directions[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                       {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;
    int position = calculatePosition(x, y);


    if (x > 0 && y > 0 && x < 9 && y < 9) {
      Piece piece = boardManager[position];
      if (piece.pieceType != EMPTY) {
        if ((pieceColor && piece.isWhite()) ||
            ((!pieceColor) && (!piece.isWhite()))) {
          continue;
        }
      }
      move.moveSquare = position;
      move.capturedPiece = boardManager[position];
      allPseudoMoves.push_back(move);
    }
  }

  if (!isKingInCheck(pieceColor, boardManager)) {
    move.capturedPiece.pieceType = EMPTY;
    move.moveType = CASTLING;

    // KINGSIDE
    if ((pieceColor && boardManager.boardSettings.whiteKingSide) ||
        (!pieceColor && boardManager.boardSettings.blackKingSide)) {
      int x = startSquare.first;
      int y = startSquare.second;
      bool canCastle = true;

      for (int i = 1; i < 3; i++) {
        int position = calculatePosition(x + i, y);

        if (boardManager[position].pieceType != EMPTY ||
            isSquareAttacked({x + i, y}, boardManager, pieceColor)) {
          canCastle = false;
          break;
        }
      }

      if (canCastle) {
        move.moveSquare = calculatePosition(x + 2, y);
        allPseudoMoves.push_back(move);
      }
    }

    // QUEENSIDE
    if ((pieceColor && boardManager.boardSettings.whiteQueenSide) ||
        (!pieceColor && boardManager.boardSettings.blackQueenSide)) {
      int x = startSquare.first;
      int y = startSquare.second;
      bool canCastle = true;

      for (int i = 1; i < 4; i++) {
        int position = calculatePosition(x - i, y);

        if (boardManager[position].pieceType != EMPTY) {
          canCastle = false;
          break;
        }

        if (i < 3) {
          if (isSquareAttacked({x - i, y}, boardManager, pieceColor)) {
            canCastle = false;
            break;
          }
        }
      }

      if (canCastle) {
        move.moveSquare = calculatePosition(x - 2, y);
        allPseudoMoves.push_back(move);
      }
    }
  }
}

void getAllPossibleKnightMoves(std::pair<int, int> startSquare,
                               Board& boardManager,
                               AllPseudoMoves& allPseudoMoves,
                               bool pieceColor) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = pieceColor ? WN : BN;

  std::pair<int, int> directions[8] = {{-2, -1}, {-1, -2}, {1, -2}, {2, -1},
                                       {2, 1},   {1, 2},   {-1, 2}, {-2, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;
    int position = calculatePosition(x, y);

    if (x > 0 && y > 0 && x < 9 && y < 9) {
      Piece piece = boardManager[position];
      if (piece.pieceType != EMPTY) {
        if ((pieceColor && piece.isWhite()) ||
            ((!pieceColor) && (!piece.isWhite()))) {
          continue;
        }
      }
      move.moveSquare = position;
      move.capturedPiece = boardManager[position];
      allPseudoMoves.push_back(move);
    }
  }
}

void getAllPossiblePawnMoves(std::pair<int, int> startSquare,
                             Board& boardManager,
                             AllPseudoMoves& allPseudoMoves, bool pieceColor) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);

  std::pair<int, int> directions[4] = {{0, 1}, {-1, 1}, {1, 1}, {0, 2}};

  for (const auto& dir : directions) {
    Move move{};
    move.square = old_position;
    move.movingPiece.pieceType = pieceColor ? WP : BP;

    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    if (!pieceColor) {
      x = startSquare.first - dir.first;
      y = startSquare.second - dir.second;
    }

    int position = calculatePosition(x, y);

    if (x > 0 && y > 0 && x < 9 && y < 9) {
      Piece piece = boardManager[calculatePosition(x, y)];
      if (dir.first != 0 && dir.second != 0) {
        if (piece.pieceType != EMPTY) {
          if ((pieceColor && piece.isWhite()) ||
              ((!pieceColor) && (!piece.isWhite()))) {
            continue;
          }
          if ((pieceColor && y == 8) || (!pieceColor && y == 1)) {
            move.moveType = PROMOTION;
            for (int promotionIndex = 0; promotionIndex < 4; promotionIndex++) {
              move.promotionPiece.pieceType =
                  (pieceColor ? whitePawnPossiblePromotions[promotionIndex]
                              : blackPawnPossiblePromotions[promotionIndex]);
              move.moveSquare = position;
              move.capturedPiece = boardManager[position];
              allPseudoMoves.push_back(move);
            }
            continue;
          }

          move.moveSquare = position;
          move.capturedPiece = boardManager[position];
          allPseudoMoves.push_back(move);
        } else {
          if (boardManager.boardSettings.epSquare != 100 &&
              calculatePosition(x, y) == boardManager.boardSettings.epSquare) {
            move.moveType = EN_PASSANT;
            move.moveSquare = position;
            move.capturedPiece = boardManager[position];
            allPseudoMoves.push_back(move);
          }
        }
      } else if (dir.first == 0 && dir.second != 0 && piece.pieceType == EMPTY) {
        if (dir.second == 2) {
          if ((pieceColor && startSquare.second == 2) ||
              (!pieceColor && startSquare.second == 7)) {
            if (boardManager[calculatePosition(
                    x, startSquare.second + (pieceColor ? 1 : -1))].pieceType == EMPTY) {
              move.moveSquare = position;
              move.capturedPiece = boardManager[position];
              allPseudoMoves.push_back(move);
            }
          }
          continue;
        }

        if ((pieceColor && y == 8) || (!pieceColor && y == 1)) {
          move.moveType = PROMOTION;
          for (int promotionIndex = 0; promotionIndex < 4; promotionIndex++) {
            move.promotionPiece.pieceType =
                (pieceColor ? whitePawnPossiblePromotions[promotionIndex]
                            : blackPawnPossiblePromotions[promotionIndex]);
            move.moveSquare = position;
            move.capturedPiece = boardManager[position];
            allPseudoMoves.push_back(move);
          }
          continue;
        }

        move.moveSquare = position;
        move.capturedPiece = boardManager[position];
        allPseudoMoves.push_back(move);
      }
    }
  }
}
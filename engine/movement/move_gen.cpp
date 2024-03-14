//
// Created by Marvin Becker on 05.03.24.
//
#include "./move_gen.h"

PseudoLegalMoves moveGenUtils::getAllPseudoLegalMoves(Board& board, bool player) {
  PseudoLegalMoves allPseudoMoves;
  for (int y = 1; y < 9; y++) {
    for (int x = 1; x < 9; x++) {
      Piece piece = board[calculatePosition(x, y)];
      if (piece.pieceType != EMPTY) {
        if ((piece.isWhite() && player) || (!piece.isWhite() && !player)) {
          switch (piece.pieceType) {
            case BP:
            case WP:
              getAllPossiblePawnMoves({x, y}, board, allPseudoMoves, player);
              break;
            case BN:
            case WN:
              getAllPossibleKnightMoves({x, y}, board, allPseudoMoves, player);
              break;
            case BB:
            case WB:
              getAllPossibleBishopMoves({x, y}, board, allPseudoMoves, player);
              break;
            case BR:
            case WR:
              getAllPossibleRookMoves({x, y}, board, allPseudoMoves, player);
              break;
            case BQ:
            case WQ:
              getAllPossibleQueenMoves({x, y}, board, allPseudoMoves, player);
              break;
            case BK:
            case WK:
              getAllPossibleKingMoves({x, y}, board, allPseudoMoves, player);
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

void moveGenUtils::getAllPossibleRookMoves(std::pair<int, int> startSquare, Board& board,
                                           PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
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
      move.capturedPiece = board[position];

      if (board[position].pieceType == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (board[position].pieceType != EMPTY) {
        if ((pieceColor && board[position].isWhite()) || (!pieceColor && !board[position].isWhite())) {
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

void moveGenUtils::getAllPossibleBishopMoves(std::pair<int, int> startSquare, Board& board,
                                             PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
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
      move.capturedPiece = board[position];

      if (board[position].pieceType == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (board[position].pieceType != EMPTY) {
        if ((pieceColor && board[position].isWhite()) || (!pieceColor && !board[position].isWhite())) {
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

void moveGenUtils::getAllPossibleQueenMoves(std::pair<int, int> startSquare, Board& board,
                                            PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = pieceColor ? WQ : BQ;

  std::pair<int, int> directions[8] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculatePosition(x, y);
      move.moveSquare = position;
      move.capturedPiece = board[position];

      if (board[position].pieceType == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (board[position].pieceType != EMPTY) {
        if ((pieceColor && board[position].isWhite()) || (!pieceColor && !board[position].isWhite())) {
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

void moveGenUtils::getAllPossibleKingMoves(std::pair<int, int> startSquare, Board& board,
                                           PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = pieceColor ? WK : BK;

  std::pair<int, int> directions[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;
    int position = calculatePosition(x, y);

    if (x > 0 && y > 0 && x < 9 && y < 9) {
      Piece piece = board[position];
      if (piece.pieceType != EMPTY) {
        if ((pieceColor && piece.isWhite()) || ((!pieceColor) && (!piece.isWhite()))) {
          continue;
        }
      }
      move.moveSquare = position;
      move.capturedPiece = piece;
      allPseudoMoves.push_back(move);
    }
  }

  if (!board.isKingInCheck(pieceColor)) {
    move.capturedPiece.pieceType = EMPTY;
    move.moveType = CASTLING;

    // KINGSIDE
    if ((pieceColor && board.boardSettings.whiteKingSide) || (!pieceColor && board.boardSettings.blackKingSide)) {
      int x = startSquare.first;
      int y = startSquare.second;
      bool canCastle = true;

      for (int i = 1; i < 3; i++) {
        int position = calculatePosition(x + i, y);

        if (board[position].pieceType != EMPTY || board.isSquareAttacked({x + i, y}, pieceColor)) {
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
    if ((pieceColor && board.boardSettings.whiteQueenSide) || (!pieceColor && board.boardSettings.blackQueenSide)) {
      int x = startSquare.first;
      int y = startSquare.second;
      bool canCastle = true;

      for (int i = 1; i < 4; i++) {
        int position = calculatePosition(x - i, y);

        if (board[position].pieceType != EMPTY) {
          canCastle = false;
          break;
        }

        if (i < 3) {
          if (board.isSquareAttacked({x - i, y}, pieceColor)) {
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

void moveGenUtils::getAllPossibleKnightMoves(std::pair<int, int> startSquare, Board& board,
                                             PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
  int old_position = calculatePosition(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = pieceColor ? WN : BN;

  std::pair<int, int> directions[8] = {{-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}};

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;
    int position = calculatePosition(x, y);

    if (x > 0 && y > 0 && x < 9 && y < 9) {
      Piece piece = board[position];
      if (piece.pieceType != EMPTY) {
        if ((pieceColor && piece.isWhite()) || ((!pieceColor) && (!piece.isWhite()))) {
          continue;
        }
      }
      move.moveSquare = position;
      move.capturedPiece = board[position];
      allPseudoMoves.push_back(move);
    }
  }
}

void moveGenUtils::getAllPossiblePawnMoves(std::pair<int, int> startSquare, Board& board,
                                           PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
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
      Piece piece = board[calculatePosition(x, y)];
      if (dir.first != 0 && dir.second != 0) {
        if (piece.pieceType != EMPTY) {
          if ((pieceColor && piece.isWhite()) || ((!pieceColor) && (!piece.isWhite()))) {
            continue;
          }
          if ((pieceColor && y == 8) || (!pieceColor && y == 1)) {
            move.moveType = PROMOTION;
            for (int promotionIndex = 0; promotionIndex < 4; promotionIndex++) {
              move.promotionPiece.pieceType = (pieceColor ? whitePawnPossiblePromotions[promotionIndex]
                                                          : blackPawnPossiblePromotions[promotionIndex]);
              move.moveSquare = position;
              move.capturedPiece = board[position];
              allPseudoMoves.push_back(move);
            }
            continue;
          }

          move.moveSquare = position;
          move.capturedPiece = board[position];
          allPseudoMoves.push_back(move);
        } else {
          if (board.boardSettings.epSquare != 100 && calculatePosition(x, y) == board.boardSettings.epSquare) {
            move.moveType = EN_PASSANT;
            move.moveSquare = position;
            move.capturedPiece = board[position];
            allPseudoMoves.push_back(move);
          }
        }
      } else if (dir.first == 0 && dir.second != 0 && piece.pieceType == EMPTY) {
        if (dir.second == 2) {
          if ((pieceColor && startSquare.second == 2) || (!pieceColor && startSquare.second == 7)) {
            if (board[calculatePosition(x, startSquare.second + (pieceColor ? 1 : -1))].pieceType == EMPTY) {
              move.moveSquare = position;
              move.capturedPiece = board[position];
              allPseudoMoves.push_back(move);
            }
          }
          continue;
        }

        if ((pieceColor && y == 8) || (!pieceColor && y == 1)) {
          move.moveType = PROMOTION;
          for (int promotionIndex = 0; promotionIndex < 4; promotionIndex++) {
            move.promotionPiece.pieceType = (pieceColor ? whitePawnPossiblePromotions[promotionIndex]
                                                        : blackPawnPossiblePromotions[promotionIndex]);
            move.moveSquare = position;
            move.capturedPiece = board[position];
            allPseudoMoves.push_back(move);
          }
          continue;
        }

        move.moveSquare = position;
        move.capturedPiece = board[position];
        allPseudoMoves.push_back(move);
      }
    }
  }
}
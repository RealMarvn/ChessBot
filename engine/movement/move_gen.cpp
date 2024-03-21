//
// Created by Marvin Becker on 05.03.24.
//
#include "./move_gen.h"

PseudoLegalMoves moveGenUtils::getAllPseudoLegalMoves(Board& board, bool player) {
  PseudoLegalMoves allPseudoMoves;
  for (int y = 1; y < 9; y++) {
    for (int x = 1; x < 9; x++) {
      Piece piece = board[calculateSquare(x, y)];
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

template <int arraySize>
inline static void getAllLinearMoves(std::pair<int, int>& startSquare, Board& board, PseudoLegalMoves& allPseudoMoves,
                                     bool pieceColor, PieceType movingPiece,
                                     std::array<std::pair<int, int>, arraySize> directions) {
  int old_position = calculateSquare(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = movingPiece;

  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;

    // Every direction until the board ends.
    while (x > 0 && y > 0 && x < 9 && y < 9) {
      int position = calculateSquare(x, y);
      move.moveSquare = position;
      move.capturedPiece = board[position];

      if (board[position].pieceType == EMPTY) {
        allPseudoMoves.push_back(move);
      } else if (board[position].pieceType != EMPTY) {
        // Check if it is your own piece.
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

void moveGenUtils::getAllPossibleRookMoves(std::pair<int, int> startSquare, Board& board,
                                           PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
  std::array<std::pair<int, int>, 4> directions = {std::pair(-1, 0), std::pair(1, 0), std::pair(0, -1),
                                                   std::pair(0, 1)};

  getAllLinearMoves<4>(startSquare, board, allPseudoMoves, pieceColor, (pieceColor ? WR : BR), directions);
}

void moveGenUtils::getAllPossibleBishopMoves(std::pair<int, int> startSquare, Board& board,
                                             PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
  std::array<std::pair<int, int>, 4> directions = {std::pair(-1, -1), std::pair(-1, 1), std::pair(1, -1),
                                                   std::pair(1, 1)};

  getAllLinearMoves<4>(startSquare, board, allPseudoMoves, pieceColor, (pieceColor ? WB : BB), directions);
}

void moveGenUtils::getAllPossibleQueenMoves(std::pair<int, int> startSquare, Board& board,
                                            PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
  std::array<std::pair<int, int>, 8> directions = {
      std::pair(-1, 0),  std::pair(1, 0),  std::pair(0, -1), std::pair(0, 1),
      std::pair(-1, -1), std::pair(-1, 1), std::pair(1, -1), std::pair(1, 1),
  };

  getAllLinearMoves<8>(startSquare, board, allPseudoMoves, pieceColor, (pieceColor ? WQ : BQ), directions);
}

void moveGenUtils::getAllPossibleKingMoves(std::pair<int, int> startSquare, Board& board,
                                           PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
  int old_position = calculateSquare(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = pieceColor ? WK : BK;

  std::pair<int, int> directions[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

  // Only run once. The king can only move one square.
  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;
    int position = calculateSquare(x, y);

    // If it is in the board.
    if (x > 0 && y > 0 && x < 9 && y < 9) {
      Piece piece = board[position];
      if (piece.pieceType != EMPTY) {
        // Check if it is your own piece.
        if ((pieceColor && piece.isWhite()) || ((!pieceColor) && (!piece.isWhite()))) {
          continue;
        }
      }
      move.moveSquare = position;
      move.capturedPiece = piece;
      allPseudoMoves.push_back(move);
    }
  }

  // Add casteling moves. King has to be save.
  if (!board.isKingInCheck(pieceColor)) {
    move.capturedPiece.pieceType = EMPTY;
    move.moveType = CASTLING;

    // KINGSIDE
    if ((pieceColor && board.boardSettings.whiteKingSide) || (!pieceColor && board.boardSettings.blackKingSide)) {
      int x = startSquare.first;
      int y = startSquare.second;
      bool canCastle = true;

      // Move two squares to the right.
      for (int i = 1; i < 3; i++) {
        int position = calculateSquare(x + i, y);

        // Check if the square is not empty or attacked.
        if (board[position].pieceType != EMPTY || board.isSquareAttacked({x + i, y}, pieceColor)) {
          // Turn of casteling if so.
          canCastle = false;
          break;
        }
      }

      if (canCastle) {
        move.moveSquare = calculateSquare(x + 2, y);
        allPseudoMoves.push_back(move);
      }
    }

    // QUEENSIDE
    if ((pieceColor && board.boardSettings.whiteQueenSide) || (!pieceColor && board.boardSettings.blackQueenSide)) {
      int x = startSquare.first;
      int y = startSquare.second;
      bool canCastle = true;

      // Move three squares to the left.
      for (int i = 1; i < 4; i++) {
        int position = calculateSquare(x - i, y);

        // If square is not empty castling is not allowed.
        if (board[position].pieceType != EMPTY) {
          canCastle = false;
          break;
        }

        // Only check the squares the king moves through if they are attacked.
        if (i < 3) {
          if (board.isSquareAttacked({x - i, y}, pieceColor)) {
            canCastle = false;
            break;
          }
        }
      }

      if (canCastle) {
        move.moveSquare = calculateSquare(x - 2, y);
        allPseudoMoves.push_back(move);
      }
    }
  }
}

void moveGenUtils::getAllPossibleKnightMoves(std::pair<int, int> startSquare, Board& board,
                                             PseudoLegalMoves& allPseudoMoves, bool pieceColor) {
  int old_position = calculateSquare(startSquare.first, startSquare.second);

  Move move{};
  move.square = old_position;
  move.movingPiece.pieceType = pieceColor ? WN : BN;

  std::pair<int, int> directions[8] = {{-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}};

  // Knight can only move in 8 directions.
  for (const auto& dir : directions) {
    int x = startSquare.first + dir.first;
    int y = startSquare.second + dir.second;
    int position = calculateSquare(x, y);

    // If it is in the board.
    if (x > 0 && y > 0 && x < 9 && y < 9) {
      Piece piece = board[position];
      if (piece.pieceType != EMPTY) {
        // Check if you capture your own piece.
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
  int old_position = calculateSquare(startSquare.first, startSquare.second);

  std::pair<int, int> directions[4] = {{0, 1}, {-1, 1}, {1, 1}, {0, 2}};
  // Pawn only has 4 possible moves.
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

    int position = calculateSquare(x, y);

    // If it is in the board.
    if (x > 0 && y > 0 && x < 9 && y < 9) {
      Piece piece = board[calculateSquare(x, y)];
      // If it is a diagonal jump.
      if (dir.first != 0 && dir.second != 0) {
        // If there is a piece.
        if (piece.pieceType != EMPTY) {
          // If there is your own piece.
          if ((pieceColor && piece.isWhite()) || ((!pieceColor) && (!piece.isWhite()))) {
            continue;
          }

          // Check for promotion on rank 8 or 1 with capture.
          if ((pieceColor && y == 8) || (!pieceColor && y == 1)) {
            move.moveType = PROMOTION;
            // Add all possible promotions.
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
          // Check for EP
          if (board.boardSettings.epSquare != 100 && calculateSquare(x, y) == board.boardSettings.epSquare) {
            move.moveType = EN_PASSANT;
            move.moveSquare = position;
            move.capturedPiece = board[position];
            allPseudoMoves.push_back(move);
          }
        }
      } else if (dir.first == 0 && dir.second != 0 && piece.pieceType == EMPTY) {  // If jump is just forward.
        if (dir.second == 2) {
          // Check if allowed to do the double jump.
          if ((pieceColor && startSquare.second == 2) || (!pieceColor && startSquare.second == 7)) {
            // Check if there is a piece on the destination.
            if (board[calculateSquare(x, startSquare.second + (pieceColor ? 1 : -1))].pieceType == EMPTY) {
              move.moveSquare = position;
              move.capturedPiece = board[position];
              allPseudoMoves.push_back(move);
            }
          }
          continue;
        }

        // Add promotions.
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

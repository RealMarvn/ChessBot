#include "./chess_bot.h"


int pieceValue[6] = {100, 300, 350, 500, 900, 1000};

int eval(const BoardManager& boardManager) {
  int number = 0;
  for (int i = 1; i < 65; i++) {
    piece piece = boardManager.board[i];
    if (piece != EMPTY) {
      number += (isWhitePiece(piece) ? 1 : -1) * pieceValue[piece % 7];
    }
  }
  return number;
}

int search(BoardManager& boardManager, int depth, int ply, Move& bestMove) {
  if (depth <= 0) {
    return eval(boardManager);
  }

  const auto moveList =
      getAllPseudoLegalMoves(boardManager, boardManager.player == WHITE);

  int legalMoves = 0;
  int bestScore = -INT_MAX;

  for (int i = 0; i < moveList.index; i++) {
    int score;

    boardManager.makeMove(moveList.move_list[i]);

    if (!isKingInCheck(boardManager.player != WHITE, boardManager)) {
      score = -search(boardManager, depth - 1, ply + 1, bestMove);
      legalMoves++;
    }
    boardManager.popLastMove();

    if (legalMoves == 0) {
      if (isKingInCheck(boardManager.player == WHITE, boardManager)) {
        return -INT_MAX + ply;  // checkmate
      } else {
        return 0;  // stalemate
      }
    } else {
      if (score > bestScore) {
        bestScore = score;
        if (ply == 0) {  // root
          bestMove = moveList.move_list[i];
        }
      }
    }
  }

  return bestScore;
}

Move searchBestNextMove(BoardManager& boardManager, int depth) {
  Move move;
  search(boardManager, depth, 0, move);
  return move;
}
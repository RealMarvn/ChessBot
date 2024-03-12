#include "./chess_bot.h"

int eval(const BoardManager& boardManager) {
  int number = 0;
  for (int i = 1; i < 65; i++) {
    Piece piece = boardManager.board[i];
    number += piece.getMaterialValue();
  }
  return number;
}

int search(BoardManager& boardManager, int depth, int alpha, int beta, int ply,
           Move& bestMove) {
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
      score =
          -search(boardManager, depth - 1, -beta, -alpha, ply + 1, bestMove);
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

      if (bestScore > alpha) {
        alpha = bestScore;
      }

      if (alpha >= beta) {
        break;  // beta killen
      }
    }
  }

  return bestScore;
}

Move searchBestNextMove(BoardManager& boardManager, int depth) {
  Move move;
  search(boardManager, depth, -INT_MAX, INT_MAX, 0, move);
  return move;
}
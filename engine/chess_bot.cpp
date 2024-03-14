#include "./chess_bot.h"

int ChessBot::eval(Board& board) {
  int number = 0;
  for (int i = 1; i < 65; i++) {
    Piece piece = board[i];
    number += piece.getMaterialValue();
  }
  return number;
}

int ChessBot::search(Board& boardManager, int depth, int alpha, int beta, int ply,
           Move& bestMove) {
  if (depth <= 0) {
    return eval(boardManager);
  }

  auto moveList =
      moveGenUtils::getAllPseudoLegalMoves(boardManager, boardManager.player == WHITE);

  int legalMoves = 0;
  int bestScore = -INT_MAX;


  for (Move& move : moveList) {
    int score;

    boardManager.makeMove(move);

    if (!boardManager.isKingInCheck(boardManager.player != WHITE)) {
      score =
          -search(boardManager, depth - 1, -beta, -alpha, ply + 1, bestMove);
      legalMoves++;
    }
    boardManager.popLastMove();

    if (legalMoves == 0) {
      if (boardManager.isKingInCheck(boardManager.player == WHITE)) {
        return -INT_MAX + ply;  // checkmate
      } else {
        return 0;  // stalemate
      }
    } else {
      if (score > bestScore) {
        bestScore = score;
        if (ply == 0) {  // root
          bestMove = move;
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

Move ChessBot::searchBestNextMove(Board& board, int depth) {
  Move move;
  search(board, depth, -INT_MAX, INT_MAX, 0, move);
  return move;
}
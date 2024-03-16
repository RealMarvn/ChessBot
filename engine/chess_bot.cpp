#include "./chess_bot.h"

int ChessBot::eval(Board& board) {
  int materialValue = 0;
  int mg[2];
  int eg[2];
  int gamePhase = 0;
  for (int i = 0; i < 64; i++) {
    Piece piece = board[i];
    materialValue += piece.getMaterialValue();
        switch (piece.pieceType) {
          case WP:
            mg[0] += mg_pawn_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case WN:
            mg[0] += mg_knight_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case WB:
            mg[0] += mg_bishop_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case WR:
            mg[0] += mg_rook_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case WQ:
            mg[0] += mg_queen_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case WK:
            mg[0] += mg_king_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case BP:
            mg[1] += mg_pawn_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case BN:
            mg[1] += mg_knight_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case BB:
            mg[1] += mg_bishop_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case BR:
            mg[1] += mg_rook_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case BQ:
            mg[1] += mg_queen_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case BK:
            mg[1] += mg_king_table[i];
            gamePhase += gamePhaseInc[piece.pieceType];
            break;
          case EMPTY:
            break;
        }
  }

  int mgScore = mg[0] - mg[1];
  int egScore = eg[0] - eg[1];
  int mgPhase = gamePhase;
  if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
  int egPhase = 24 - mgPhase;

  return ((mgScore * mgPhase + egScore * egPhase) / 24) + materialValue;
}

int ChessBot::search(Board& boardManager, int depth, int alpha, int beta, int ply, Move& bestMove) {
  if (depth <= 0) {
    return eval(boardManager);
  }

  auto moveList = moveGenUtils::getAllPseudoLegalMoves(boardManager, boardManager.player == WHITE);

  int legalMoves = 0;
  int bestScore = -INT_MAX;

  for (Move& move : moveList) {
    int score;

    boardManager.makeMove(move);

    if (!boardManager.isKingInCheck(boardManager.player != WHITE)) {
      score = -search(boardManager, depth - 1, -beta, -alpha, ply + 1, bestMove);
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
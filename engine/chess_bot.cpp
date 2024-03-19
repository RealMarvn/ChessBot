#include "./chess_bot.h"

int ChessBot::eval(Board& board) {
  int mg_MaterialValue = 0;
  int eg_MaterialValue = 0;
  int mg[2];
  int eg[2];
  int gamePhase = 0;
  for (int i = 0; i < 64; i++) {
    Piece piece = board[i];
    mg_MaterialValue += piece.getMaterialValue(false);
    eg_MaterialValue += piece.getMaterialValue(true);
    switch (piece.pieceType) {
      case WP:
        mg[0] += -1 * mg_pawn_table[i];
        eg[0] += -1 * eg_pawn_table[i];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case WN:
        mg[0] += -1 * mg_knight_table[i];
        eg[0] += -1 * eg_knight_table[i];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case WB:
        mg[0] += mg_bishop_table[i];
        eg[0] += eg_bishop_table[i];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case WR:
        mg[0] += mg_rook_table[i];
        eg[0] += eg_rook_table[i];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case WQ:
        mg[0] += mg_queen_table[i];
        eg[0] += eg_queen_table[i];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case WK:
        mg[0] += mg_king_table[i];
        eg[0] += eg_king_table[i];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case BP:
        mg[1] += mg_pawn_table[i ^ 56];
        eg[1] += eg_pawn_table[i ^ 56];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case BN:
        mg[1] += mg_knight_table[i ^ 56];
        eg[1] += eg_knight_table[i ^ 56];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case BB:
        mg[1] += mg_bishop_table[i ^ 56];
        eg[1] += eg_bishop_table[i ^ 56];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case BR:
        mg[1] += mg_rook_table[i ^ 56];
        eg[1] += eg_rook_table[i ^ 56];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case BQ:
        mg[1] += mg_queen_table[i ^ 56];
        eg[1] += eg_queen_table[i ^ 56];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case BK:
        mg[1] += mg_king_table[i ^ 56];
        eg[1] += eg_king_table[i ^ 56];
        gamePhase += gamePhaseInc[piece.pieceType];
        break;
      case EMPTY:
        break;
    }
  }

  // Calculate PSQT and game phase (CODE from https://www.chessprogramming.org/PeSTO's_Evaluation_Function) (MODIFIED by
  // MARVIN).
  int mgScore = mg[board.player == WHITE] - mg[board.player != WHITE] + mg_MaterialValue;
  int egScore = eg[board.player == WHITE] - eg[board.player != WHITE] + eg_MaterialValue;
  int mgPhase = gamePhase;
  if (mgPhase > 24) mgPhase = 24;
  int egPhase = 24 - mgPhase;

  int evaluation = ((mgScore * mgPhase + egScore * egPhase) / 24);

  // Give a bonus to side to move (TEMPO).
  return evaluation + 20;
}

int ChessBot::search(Board& boardManager, int depth, int alpha, int beta, int ply, Move& bestMove) {
  if (depth <= 0) {
    return eval(boardManager);
  }

  auto moveList = moveGenUtils::getAllPseudoLegalMoves(boardManager, boardManager.player == WHITE);

  int legalMoves = 0;
  // First best score should be the worst.
  int bestScore = -INT_MAX;

  for (Move& move : moveList) {
    int score{0};

    // Make every move and gather the value of the opponent.
    if (boardManager.makeMove(move)) {
      score = -search(boardManager, depth - 1, -beta, -alpha, ply + 1, bestMove);
      legalMoves++;
      boardManager.popLastMove();
    }

    // Set best score if the current one is less.
    if (score > bestScore) {
      bestScore = score;
      if (ply == 0) {  // Set best move if it is the root.
        bestMove = move;
      }
    }

    // ALPHA BETA PRUNING

    if (bestScore > alpha) {
      alpha = bestScore;
    }

    if (alpha >= beta) {
      break;  // beta kill
    }
  }

  // If no legal moves
  if (legalMoves == 0) {
    if (boardManager.isKingInCheck(boardManager.player == WHITE)) {
      return -INT_MAX + ply;  // checkmate
    } else {
      return 0;  // stalemate
    }
  }

  return bestScore;
}

Move ChessBot::searchBestNextMove(Board& board, int depth) {
  Move move;
  search(board, depth, -INT_MAX, INT_MAX, 0, move);
  return move;
}

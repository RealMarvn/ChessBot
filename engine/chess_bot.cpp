#include "./chess_bot.h"

int ChessBot::eval(Board& board) {
  int mg_MaterialValue = 0;
  int eg_MaterialValue = 0;
  int mg[2] = {0};
  int eg[2] = {0};
  int gamePhase = 0;
  for (int i = 0; i < 64; i++) {
    Piece piece = board[i];
    // Calculate material value.
    mg_MaterialValue += piece.getMaterialValue(false);
    eg_MaterialValue += piece.getMaterialValue(true);
    // Calculate game phase for 'Tampered Eval'.
    gamePhase += piece.getGamePhaseValue();
    // Add PSQT values.
    switch (piece.pieceType) {
      case WP:
        mg[0] += mg_pawn_table[i];
        eg[0] += eg_pawn_table[i];
        break;
      case WN:
        mg[0] += mg_knight_table[i];
        eg[0] += eg_knight_table[i];
        break;
      case WB:
        mg[0] += mg_bishop_table[i];
        eg[0] += eg_bishop_table[i];
        break;
      case WR:
        mg[0] += mg_rook_table[i];
        eg[0] += eg_rook_table[i];
        break;
      case WQ:
        mg[0] += mg_queen_table[i];
        eg[0] += eg_queen_table[i];
        break;
      case WK:
        mg[0] += mg_king_table[i];
        eg[0] += eg_king_table[i];
        break;
      case BP:
        mg[1] += mg_pawn_table[i ^ 56];
        eg[1] += eg_pawn_table[i ^ 56];
        break;
      case BN:
        mg[1] += mg_knight_table[i ^ 56];
        eg[1] += eg_knight_table[i ^ 56];
        break;
      case BB:
        mg[1] += mg_bishop_table[i ^ 56];
        eg[1] += eg_bishop_table[i ^ 56];
        break;
      case BR:
        mg[1] += mg_rook_table[i ^ 56];
        eg[1] += eg_rook_table[i ^ 56];
        break;
      case BQ:
        mg[1] += mg_queen_table[i ^ 56];
        eg[1] += eg_queen_table[i ^ 56];
        break;
      case BK:
        mg[1] += mg_king_table[i ^ 56];
        eg[1] += eg_king_table[i ^ 56];
        break;
      case EMPTY:
        break;
    }
  }

  // Calculate PSQT and game phase with tampered eval (https://www.chessprogramming.org/PeSTO's_Evaluation_Function)
  // (MODIFIED by MARVIN).
  int mgScore = mg[board.player == WHITE] - mg[board.player != WHITE] + mg_MaterialValue;
  int egScore = eg[board.player == WHITE] - eg[board.player != WHITE] + eg_MaterialValue;
  // Calculate the game phase.
  int mgPhase = gamePhase;
  if (mgPhase > 24) mgPhase = 24;
  int egPhase = 24 - mgPhase;

  int evaluation = ((mgScore * mgPhase + egScore * egPhase) / 24);

  // Give a bonus to side to move (TEMPO).
  return evaluation + 20;
}

int ChessBot::search(Board& board, int depth, int alpha, int beta, int ply, Move& bestMove) {
  if (depth <= 0) {
    // If depth reached, eval the board.
    return eval(board);
  }

  // Get all possible moves.
  auto moveList = moveGenUtils::getAllPseudoLegalMoves(board, board.player == WHITE);

  int legalMoves = 0;

  // First best score should be the worst.
  int bestScore = -INT_MAX;

  for (Move& move : moveList) {
    int score{0};

    // Make every move and gather the value of the opponent.
    if (board.makeMove(move)) {
      score = -search(board, depth - 1, -beta, -alpha, ply + 1, bestMove);
      legalMoves++;
      board.popLastMove();
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
    if (board.isKingInCheck(board.player == WHITE)) {
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

#include "./chess_bot.h"

int ChessBot::eval(Board& board) {
  int mg[2] = {0};
  int eg[2] = {0};
  int gamePhase = 0;
  for (int i = 0; i < 64; i++) {
    Piece piece = board[i];
    // Calculate game phase for 'Tampered Eval'.
    gamePhase += piece.getGamePhaseValue();
    // Add PSQT values + material value.
    switch (piece.pieceType) {
      case WP:
        mg[0] += mg_pawn_table[i ^ 56] + piece.getMaterialValue(false);
        eg[0] += eg_pawn_table[i ^ 56] + piece.getMaterialValue(true);
        break;
      case WN:
        mg[0] += mg_knight_table[i ^ 56] + piece.getMaterialValue(false);
        eg[0] += eg_knight_table[i ^ 56] + piece.getMaterialValue(true);
        break;
      case WB:
        mg[0] += mg_bishop_table[i ^ 56] + piece.getMaterialValue(false);
        eg[0] += eg_bishop_table[i ^ 56] + piece.getMaterialValue(true);
        break;
      case WR:
        mg[0] += mg_rook_table[i ^ 56] + piece.getMaterialValue(false);
        eg[0] += eg_rook_table[i ^ 56] + piece.getMaterialValue(true);
        break;
      case WQ:
        mg[0] += mg_queen_table[i ^ 56] + piece.getMaterialValue(false);
        eg[0] += eg_queen_table[i ^ 56] + piece.getMaterialValue(true);
        break;
      case WK:
        mg[0] += mg_king_table[i ^ 56] + piece.getMaterialValue(false);
        eg[0] += eg_king_table[i ^ 56] + piece.getMaterialValue(true);
        break;
      case BP:
        mg[1] += mg_pawn_table[i] + piece.getMaterialValue(false);
        eg[1] += eg_pawn_table[i] + piece.getMaterialValue(true);
        break;
      case BN:
        mg[1] += mg_knight_table[i] + piece.getMaterialValue(false);
        eg[1] += eg_knight_table[i] + piece.getMaterialValue(true);
        break;
      case BB:
        mg[1] += mg_bishop_table[i] + piece.getMaterialValue(false);
        eg[1] += eg_bishop_table[i] + piece.getMaterialValue(true);
        break;
      case BR:
        mg[1] += mg_rook_table[i] + piece.getMaterialValue(false);
        eg[1] += eg_rook_table[i] + piece.getMaterialValue(true);
        break;
      case BQ:
        mg[1] += mg_queen_table[i] + piece.getMaterialValue(false);
        eg[1] += eg_queen_table[i] + piece.getMaterialValue(true);
        break;
      case BK:
        mg[1] += mg_king_table[i] + piece.getMaterialValue(false);
        eg[1] += eg_king_table[i] + piece.getMaterialValue(true);
        break;
      case EMPTY:
        break;
    }
  }

  // Calculate PSQT and game phase with tampered eval (https://www.chessprogramming.org/PeSTO's_Evaluation_Function)
  // (MODIFIED by MARVIN).
  int mgScore = mg[board.player != WHITE] - mg[board.player == WHITE];
  int egScore = eg[board.player != WHITE] - eg[board.player == WHITE];
  // Calculate the game phase.
  int mgPhase = gamePhase;
  if (mgPhase > 24) mgPhase = 24;
  int egPhase = 24 - mgPhase;

  int evaluation = (((mgScore * mgPhase) + (egScore * egPhase)) / 24);

  // Give a bonus to side to move (TEMPO).
  return (evaluation + 20);
}

Move ChessBot::iterativeDeepening(Board& board) {
  start = std::chrono::high_resolution_clock::now();
  Move bestMove{};
  for (int i = 1;; i++) {
    Move move = searchBestNextMove(board, i);
    if (isTimeUp()) {
      std::cout << "Best move not taken from depth " << i << std::endl;
      std::cout << "Killed Move: " << move.toString() << std::endl;
      break;
    }
    std::cout << "Stored Move from depth " << i << ": " << move.toString() << std::endl;
    bestMove = move;
  }
  std::cout << "Best Move: " << bestMove.toString() << std::endl;
  return bestMove;
}

int ChessBot::search(Board& board, int depth, int alpha, int beta, int ply, Move& bestMove) {
  if (depth <= 0) {
    // If depth reached, run qsearch so you don't sacrifice your piece and eval the board.
    return quiescenceSearch(board, alpha, beta);
  }

  // If time is up kill the prozess by returning anything.
  if (isTimeUp()) {
    return -INT_MAX;
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
    } else {
      continue;
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

int ChessBot::quiescenceSearch(Board& board, int alpha, int beta) {
  // Eval the position.
  int stand_pat = eval(board);

  if (stand_pat >= beta) {
    return beta;
  }
  if (alpha < stand_pat) {
    alpha = stand_pat;
  }

  // Get all possible moves.
  auto moveList = moveGenUtils::getAllPseudoLegalMoves(board, board.player == WHITE);
  moveList.sortAllMoves();

  // First best score should be the worst.
  int bestScore = stand_pat;

  for (Move& move : moveList) {
    int score{0};

    // Filter the normal moves so I only have captures.
    if (move.capturedPiece.pieceType == EMPTY && move.moveType != EN_PASSANT) {
      continue;
    }

    // Make every move and gather the value of the opponent.
    if (board.makeMove(move)) {
      score = -quiescenceSearch(board, -beta, -alpha);
      board.popLastMove();
    } else {
      continue;
    }

    // Update best score.
    bestScore = std::max(bestScore, score);
    // if score is less than alpha continue.
    if (score <= alpha) continue;
    // set the score
    alpha = score;
    if (score >= beta) {
      break;
    }
  }

  return bestScore;
}

Move ChessBot::searchBestNextMove(Board& board, int depth) {
  Move move;
  search(board, depth, -INT_MAX, INT_MAX, 0, move);
  return move;
}

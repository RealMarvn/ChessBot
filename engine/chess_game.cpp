//
// Created by Marvin Becker on 16.03.24.
//

#include "./chess_game.h"

#ifdef DEBUG
uint64_t perft(Board& boardManager, int depth, bool player) {
  if (depth == 0) {
    return 1;
  }

  uint64_t nodes = 0;
  // Get all moves.
  auto moves = moveGenUtils::getAllPseudoLegalMoves(boardManager, player);
  for (Move& move : moves) {
    // If move is valid get the value.
    if (boardManager.makeMove(move)) {
      nodes += perft(boardManager, depth - 1, !player);
      boardManager.popLastMove();
    }
  }

  return nodes;
}

uint64_t split_perft(Board& boardManager, int depth, bool player) {
  if (depth == 0) {
    return 0;
  }

  int number = 0;

  // Generate every move for the current position.
  auto moves = moveGenUtils::getAllPseudoLegalMoves(boardManager, player);
  for (Move& move : moves) {
    if (boardManager.makeMove(move)) {
      uint64_t child_nodes = perft(boardManager, depth - 1, !player);

      number += child_nodes;
      // Print all moves a move can generate.
      std::cout << move.convertToXandY() << " - " << child_nodes << std::endl;
      boardManager.popLastMove();
    }
  }
  return number;
}
#endif

void ChessGame::start() {
  board->printCurrentBoard();
  std::string input;
  while (getline(std::cin, input)) {
    // Read in FEN notation.
    if (input[0] == 'F') {
      board->readFen(input.substr(1, input.length()));
#ifdef DEBUG
      split_perft(*board, 1, board->player == WHITE);
#endif
      board->printCurrentBoard();
      continue;
    }

    // undo the last two moves. (Bot did also move that's why)
    if (input == "undo") {
      board->popLastMove();
      board->popLastMove();
      board->printCurrentBoard();
      continue;
    }

    if (input.length() < 5) {
      std::cout << "invalid" << std::endl;
      continue;
    }

    Move playerMove = board->parseMove(input);

    // Make the move and check for CheckMate.
    if (board->tryToMovePiece(playerMove)) {
      if (board->isCheckMate(board->player == WHITE)) {
        std::cout << "CHECK MATE!" << std::endl;
        return;
      }

      // Bot can only move legal so no need to check if the move is legal.
      // Check if opponent is in check mate after bots turn.
      Move move = ChessBot::searchBestNextMove(*board, 5);
      board->makeMove(move);
      board->printCurrentBoard();

      if (board->isCheckMate(board->player == WHITE)) {
        board->printCurrentBoard();
        std::cout << "CHECK MATE!" << std::endl;
        return;
      }
    } else {
      std::cout << "invalid" << std::endl;
    }
  }
}

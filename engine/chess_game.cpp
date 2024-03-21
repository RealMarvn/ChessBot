//
// Created by Marvin Becker on 16.03.24.
//

#include "./chess_game.h"

void ChessGame::start() {
  board->printCurrentBoard();
  std::string input;
  while (getline(std::cin, input)) {
    // Read in FEN notation.
    if (input[0] == 'F') {
      board->readFen(input.substr(1, input.length()));
      board->printCurrentBoard();
      continue;
    } else if (input[0] == 'f') {
      std::cout << "Your FEN: " << board->getFen() << std::endl;
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

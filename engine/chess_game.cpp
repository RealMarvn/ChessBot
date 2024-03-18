//
// Created by Marvin Becker on 16.03.24.
//

#include "chess_game.h"

void ChessGame::start() {
  board->printCurrentBoard();
  std::string input;
  while (getline(std::cin, input)) {
    // Read in FEN notation.
    if (input[0] == 'F') {
      board->readFen(input.substr(1, input.length()));
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

    bool capture = false;
    char promotion_figure = ' ';
    char figure = input[0];
    // Use the ascii code of the char to subtract a number to get the correct number.
    int col = input[1] - 96;
    int row = input[2] - 48;

    int move_col = input[3] - 96;
    int move_row = input[4] - 48;

    // Capture can be lower or upper case.
    if (input[3] == 'x' || input[3] == 'X') {
      move_col = input[4] - 96;
      move_row = input[5] - 48;
      capture = true;
    }

    // If there is a capture the promotion is moved.
    if ((capture && input.length() == 8 && input[6] == '=')) {
      promotion_figure = input[7];
    } else if (input.length() == 7 && input[5] == '=') {
      promotion_figure = input[6];
    }

    // Make the move and check for CheckMate.
    if (board->movePiece(figure, col, row, move_col, move_row, capture, promotion_figure)) {
      if (board->isCheckMate(board->player == WHITE)) {
        std::cout << "CHECK MATE!" << std::endl;
        return;
      }

      // Bot can only move legal so no need to check if the move is legal.
      // Check if opponent is in check mate after bots turn.
      board->makeMove(bot->searchBestNextMove(*board, 5));
      board->printCurrentBoard();

      if (board->isCheckMate(board->player == WHITE)) {
        board->printCurrentBoard();
        std::cout << "CHECK MATE!" << std::endl;
        return;
      }
    }
  }
}

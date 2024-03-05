#include "./manager/board_manager.h"

#include <iostream>

int main() {
  const auto board = std::make_unique<BoardManager>();
  std::string input;

  while (getline(std::cin, input)) {

    if (input[0] == 'B') {
      board->readFen(input.substr(1, input.length()));
      continue;
    }

    if (input == "undo") {
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
    int col = input[1] - 96;
    int row = input[2] - 48;

    int move_col = input[3] - 96;
    int move_row = input[4] - 48;

    if (input[3] == 'x' || input[3] == 'X') {
      move_col = input[4] - 96;
      move_row = input[5] - 48;
      capture = true;
    }

    if ((capture && input.length() == 8 && input[6] == '=')) {
      promotion_figure = input[7];
    } else if (input.length() == 7 && input[5] == '=') {
      promotion_figure = input[6];
    }

    board->movePiece(figure, col, row, move_col, move_row, capture,
                     promotion_figure);

    board->printCurrentBoard();
  }
  return EXIT_SUCCESS;
}

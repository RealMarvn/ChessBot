#include "./manager/board_manager.h"

#include <iostream>

int main() {
  const auto board = std::make_unique<BoardManager>();
  std::string input;
  while (getline(std::cin, input)) {

    if (input.length() < 5) {
      std::cout << "invalid" << std::endl;
      continue;
    }

    bool capture = false;
    char figure = input[0];
    const int col = input[1] - 96;
    const int row = input[2] - 48;

    int move_col = input[3] - 96;
    int move_row = input[4] - 48;

    if (input[3] == 'x' || input[3] == 'X') {
      move_col = input[4] - 96;
      move_row = input[5] - 48;
      capture = true;
    }

    board->movePiece(figure, col, row, move_col, move_row, capture);
    board->printCurrentBoard();
  }
  return EXIT_SUCCESS;
}

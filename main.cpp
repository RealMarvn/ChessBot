#include "./manager/board_manager.h"

#include <iostream>

int main(int argc, char *argv[]) {
  const auto board = std::make_unique<BoardManager>();
  std::string input;
  bool debug = false;

  if (argc == 2) {
    std::string arg2 = argv[1];
    if (arg2 == "debug") {
      debug = true;
    }
  }

  while (getline(std::cin, input)) {

    if (input == "undo") {
      board->popLastMove();
      board->printCurrentBoard(debug);
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

    if (debug) {

      if (input[0] == 'B') {
        auto test = input.substr(1, 64);
        board->readInBoard(input.substr(1, 64));
        continue;
      } else {
        figure = input[1];
        col = input[2] - 96;
        row = input[3] - 48;

        move_col = input[4] - 96;
        move_row = input[5] - 48;

        if (input[4] == 'x' || input[4] == 'X') {
          move_col = input[5] - 96;
          move_row = input[6] - 48;
          capture = true;
        }

        if ((capture && input.length() == 9 && input[7] == '=')) {
          promotion_figure = input[8];
        } else if (input.length() == 8 && input[6] == '=') {
          promotion_figure = input[7];
        }
      }
    }


    board->movePiece(figure, col, row, move_col, move_row, capture,
                     promotion_figure);

    board->printCurrentBoard(debug);
  }
  return EXIT_SUCCESS;
}

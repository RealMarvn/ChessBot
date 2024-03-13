#include <iostream>

#include "./engine/chess_bot.h"

uint64_t perft(Board& boardManager, int depth, bool player) {
  if (depth == 0) {
    return 1;
  }

  uint64_t nodes = 0;
  auto moves = getAllPseudoLegalMoves(boardManager, player);
  for (int i = 0; i < moves.index; i++) {
    boardManager.makeMove(moves.move_list[i]);

    if (isKingInCheck(player, boardManager)) {
      boardManager.popLastMove();
      continue;
    }
    nodes += perft(boardManager, depth - 1, !player);
    boardManager.popLastMove();
  }

  return nodes;
}

int split_perft(Board& boardManager, int depth, bool player) {
  if (depth == 0) {
    return 0;
  }

  int number = 0;

  // Generiere die Züge für die aktuelle Position
  auto moves = getAllPseudoLegalMoves(boardManager, player);
  for (int i = 0; i < moves.index; i++) {
    boardManager.makeMove(moves.move_list[i]);

    if (isKingInCheck(player, boardManager)) {
      boardManager.popLastMove();
      continue;
    }

    uint64_t child_nodes = perft(boardManager, depth - 1, !player);

    number += child_nodes;
    std::cout << moves.move_list[i].convertToXandY() << " - " << child_nodes
              << std::endl;
    boardManager.popLastMove();
  }
  return number;
}

int main() {
  const auto board = std::make_unique<Board>();
  std::string input;
  std::cout << "Perft: " << perft(*board, 5, board->player == WHITE) << std::endl;
  while (getline(std::cin, input)) {
    if (input[0] == 'B') {
      board->readFen(input.substr(1, input.length()));
      std::cout << "Perft: " << perft(*board, 4, board->player == WHITE) << std::endl;
      // auto split_num = split_perft(*board, depth, board->player == WHITE);
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

    auto move = searchBestNextMove(*board, 5);
    board->makeMove(move);
    board->printCurrentBoard();
  }
  return EXIT_SUCCESS;
}

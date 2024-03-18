//
// Created by Marvin Becker on 13.03.24.
//

#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "../engine/chess_bot.h"

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

TEST(MoveGenTest, PerftTest) {
  // PERFT file holen.
  std::ifstream epd_file("/Users/marvin/CLionProjects/pk1-pruefung/tests/data/perft-positions.epd");
  ASSERT_TRUE(epd_file.good()) << "Der Pfad der testing suite ist falsch. Bitte anpassen!";
  Board myBoard;

  std::string line;
  // Read in perft file per line.
  while (std::getline(epd_file, line)) {
    std::istringstream ss(line);
    std::string setting;
    std::vector<std::string> settings;

    // Cut the line into sections.
    while (std::getline(ss, setting, ';')) {
      settings.push_back(setting);
    }

    // Read in FEN.
    myBoard.readFen(settings[0]);

    // Check FEN!
    auto result = perft(myBoard, 4, myBoard.player == WHITE);
    ASSERT_EQ(result, std::stoi(settings[4].substr(3)));
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
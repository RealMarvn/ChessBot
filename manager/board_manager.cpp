//
// Created by Marvin Becker on 15.12.23.
//

#include "board_manager.h"

BoardManager::BoardManager() {
  player = WHITE;
  readFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

bool BoardManager::popLastMove() {
  if (moves.empty()) {
    return false;
  }
  Move last_move = moves.back();
  moves.pop_back();
  board[last_move.position] = last_move.capturedFigure;
  board[last_move.old_position] = last_move.figure;
  player = player == WHITE ? BLACK : WHITE;
  return true;
}

// TODO: Work in Progress
bool BoardManager::canCastle(int x, int y, int move_x, int move_y) {
  Piece piece = board[calculatePosition(x, y)];
  Piece movePiece = board[calculatePosition(move_x, move_y)];

  if (piece.moved || movePiece.moved) {
    return true;
  }

  if (x < move_x) {
    if ((board[calculatePosition(x + 1, y)].figure == ' ') &&
        (board[calculatePosition(x + 2, y)].figure == ' ')) {
    }
  }

  if (x > move_x) {
    if ((board[calculatePosition(x - 1, y)].figure == ' ') &&
        (board[calculatePosition(x - 2, y)].figure == ' ')) {
    }
  }
  return false;
}

void BoardManager::saveMove(int movePosition, int position) {
  Move move{};
  move.capturedFigure = board[movePosition];
  move.figure = board[position];
  move.position = movePosition;
  move.old_position = position;
  moves.push_back(move);
}

bool BoardManager::movePiece(char fig, int x, int y, int move_x, int move_y,
                             bool capture, char promotion_figure) {
  if ((player == BLACK && isupper(fig)) || (player == WHITE && islower(fig))) {
    std::cout << "invalid" << std::endl;
    return false;
  }

  if (!canMove(fig, x, y, move_x, move_y, capture)) {
    std::cout << "invalid" << std::endl;
    return false;
  }

  bool promotion = false;
  int position = calculatePosition(x, y);
  int movePosition = calculatePosition(move_x, move_y);

  if (promotion_figure != ' ') {
    if (!canPawnPromote(player == WHITE, fig, promotion_figure, movePosition)) {
      std::cout << "invalid" << std::endl;
      return false;
    }
    promotion = true;
  }

  saveMove(movePosition, position);

  board[position].moved = true;
  board[movePosition] = board[position];
  board[position].figure = ' ';
  player = player == WHITE ? BLACK : WHITE;

  if (promotion) {
    board[movePosition].figure = promotion_figure;
  }

  if (isKingInDanger(false)) {
    std::cout << "King is in check" << std::endl;
  } else {
    std::cout << "No one is in check" << std::endl;
  }

  return true;
}

bool BoardManager::canMove(char fig, int x, int y, int move_x, int move_y,
                           bool capture) {
  int position = calculatePosition(x, y);
  int move = calculatePosition(move_x, move_y);

  // Check if position is out of bounds!
  if (position < 1 || position > 64 || move < 1 || move > 64) {
    return false;
  }

  // Check if figure is really that figure.
  if (board[position].figure != fig) {
    return false;
  }

  //  if (canCastle(x, y, move_x, move_y)) {
  //    return true;
  //  }

  // Check if capture an empty field or a field without a capture
  if ((board[move].figure != ' ' && !capture) ||
      (capture && board[move].figure == ' ')) {
    return false;
  }

  if (capture) {
    // Check if you try to capture your own team.
    if ((isupper(board[position].figure) && isupper(board[move].figure)) ||
        (islower(board[position].figure) && islower(board[move].figure))) {
      return false;
    }
  }

  switch (tolower(fig)) {
    case 'p':
      if (isPathClear(x, y, move_x, move_y, board)) {
        return canPawnMove(x, y, move_x, move_y, capture,
                           board[calculatePosition(x, y)].moved, isupper(fig));
      }
      break;
    case 'k':
      if (isPathClear(x, y, move_x, move_y, board)) {
        return canKingMove(x, y, move_x, move_y);
      }
      break;
    case 'b':
      if (isPathClear(x, y, move_x, move_y, board)) {
        return canBishopMove(x, y, move_x, move_y);
      }
      break;
    case 'r':
      if (isPathClear(x, y, move_x, move_y, board)) {
        return canRookMove(x, y, move_x, move_y);
      }
      break;
    case 'q':
      if (isPathClear(x, y, move_x, move_y, board)) {
        return canQueenMove(x, y, move_x, move_y);
      }
      break;
    case 'n':
      return canKnightMove(x, y, move_x, move_y);
  }
  return false;
}

bool BoardManager::isPathClear(int startX, int startY, int endX, int endY,
                               Piece board[65]) {
  int dx = std::abs(endX - startX);
  int dy = std::abs(endY - startY);
  int x = startX;
  int y = startY;
  int xInc = (endX > startX) ? 1 : -1;
  int yInc = (endY > startY) ? 1 : -1;

  if (dx > dy) {
    int p = 2 * dy - dx;
    while (x != endX) {
      if (p >= 0) {
        y += yInc;
        p -= 2 * dx;
      }
      x += xInc;
      p += 2 * dy;

      if (x != endX || y != endY) {
        if (board[calculatePosition(x, y)].figure != ' ') {
          return false;
        }
      }
    }
  } else {
    int p = 2 * dx - dy;
    while (y != endY) {
      if (p >= 0) {
        x += xInc;
        p -= 2 * dy;
      }
      y += yInc;
      p += 2 * dx;

      if (x != endX || y != endY) {
        if (board[calculatePosition(x, y)].figure != ' ') {
          return false;
        }
      }
    }
  }

  return true;  // Der Pfad ist klar
}

bool BoardManager::isWhiteKingInDanger() {
  int whiteKingPositionX = 0;
  int whiteKingPositionY = 0;

  // Get King position
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      char figure = board[calculatePosition(x, y)].figure;
      if (figure == 'K') {
        whiteKingPositionX = x;
        whiteKingPositionY = y;
      }
    }
  }

  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      char figure = board[calculatePosition(x, y)].figure;
      if (islower(figure)) {
        if (canMove(figure, x, y, whiteKingPositionX, whiteKingPositionY,
                    true)) {
          return true;
        }
      }
    }
  }

  return false;
}

bool BoardManager::isBlackKingInDanger() {
  int blackKingPositionX = 0;
  int blackKingPositionY = 0;

  // Get King position
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      char figure = board[calculatePosition(x, y)].figure;
      if (figure == 'k') {
        blackKingPositionX = x;
        blackKingPositionY = y;
      }
    }
  }

  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      char figure = board[calculatePosition(x, y)].figure;
      if (isupper(figure)) {
        if (canMove(figure, x, y, blackKingPositionX, blackKingPositionY,
                    true)) {
          return true;
        }
      }
    }
  }

  return false;
}

// TODO: Can be improved
bool BoardManager::isKingInDanger(bool justReadIn) {
  int whiteKingPositionX = 0;
  int whiteKingPositionY = 0;
  int blackKingPositionX = 0;
  int blackKingPositionY = 0;

  // Get King position
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      char figure = board[calculatePosition(x, y)].figure;
      if (figure == 'k') {
        blackKingPositionX = x;
        blackKingPositionY = y;
      } else if (figure == 'K') {
        whiteKingPositionX = x;
        whiteKingPositionY = y;
      }
    }
  }

  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      char figure = board[calculatePosition(x, y)].figure;
      if (islower(figure)) {
        if (canMove(figure, x, y, whiteKingPositionX, whiteKingPositionY,
                    true)) {
          return true;
        }
      } else if (isupper(figure) && !justReadIn) {
        if (canMove(figure, x, y, blackKingPositionX, blackKingPositionY,
                    true)) {
          return true;
        }
      }
    }
  }
  return false;
}

int BoardManager::calculatePosition(int x, int y) { return (y - 1) * 8 + x; }

void BoardManager::readFen(std::string input) {
  std::vector<std::string> fenSettings;

  std::istringstream iss(input);
  for (std::string s; iss >> s;) fenSettings.push_back(s);

  player = WHITE;
  Piece piece{};
  piece.figure = ' ';
  piece.moved = false;
  board[0] = piece;

  int inputIndex = 0;
  int x = 1;
  int y = 8;
  while (input[inputIndex] != ' ' && y != 0) {
    if (x > 8) {
      x = 1;
    }

    switch (input[inputIndex]) {
      case '/':
        y--;
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
        for (int i = input[inputIndex] - '0'; i > 0; --i) {
          piece.figure = ' ';
          board[calculatePosition(x, y)] = piece;
          x++;
        }
        break;
      default:
        piece.figure = input[inputIndex];
        board[calculatePosition(x, y)] = piece;
        piece.moved = false;
        x++;
    }
    inputIndex++;
  }

  if (fenSettings[1] == "b") {
    player = BLACK;
  }

  printCurrentBoard();
}

// TODO: refactor this function to improve efficiency
void BoardManager::printPossibleMoves(char fig, int x, int y) {
  Piece saveBoard[65];
  std::memcpy(saveBoard, board, sizeof(board));
  Piece piece = board[calculatePosition(x, y)];

  for (int row = 8; row >= 1; row--) {
    for (int column = 1; column <= 8; column++) {
      if (board[calculatePosition(column, row)].figure == ' ' &&
          canMove(fig, x, y, column, row, false)) {
        // Mach den Move!
        piece.moved = true;
        board[calculatePosition(column, row)] = piece;
        board[calculatePosition(x, y)].figure = ' ';
        if ((isupper(fig) && !isWhiteKingInDanger()) ||
            (islower(fig) && !isBlackKingInDanger())) {
          std::cout << "[o]";
          std::memcpy(board, saveBoard, sizeof(saveBoard));
          continue;
        }
        std::memcpy(board, saveBoard, sizeof(saveBoard));
      }
      if (canMove(fig, x, y, column, row, true)) {
        // Mach den Move!
        piece.moved = true;
        board[calculatePosition(column, row)] = piece;
        board[calculatePosition(x, y)].figure = ' ';
        if ((isupper(fig) && !isWhiteKingInDanger()) ||
            (islower(fig) && !isBlackKingInDanger())) {
          std::cout << "[x]";
          std::memcpy(board, saveBoard, sizeof(saveBoard));
          continue;
        }
        std::memcpy(board, saveBoard, sizeof(saveBoard));
      }
      std::cout << "[" << board[calculatePosition(column, row)].figure << "]";
      std::memcpy(board, saveBoard, sizeof(saveBoard));
    }
    std::cout << std::endl;
  }
  std::memcpy(board, saveBoard, sizeof(saveBoard));
}

void BoardManager::printCurrentBoard() {
  if (player == WHITE) {
    std::cout << "Current turn: "
              << "White" << std::endl;
  } else {
    std::cout << "Current turn: "
              << "Black" << std::endl;
  }

  for (int y = 8; y >= 1; y--) {
    std::cout << y << " | ";
    for (int x = 1; x <= 8; x++) {
      std::cout << "[" << board[calculatePosition(x, y)].figure << "]";
    }
    std::cout << std::endl;
  }
  std::cout << "     a";
  for (int i = 2; i <= 8; i++) {
    std::cout << "  " << char(i + 96);
  }
  std::cout << std::endl;
}

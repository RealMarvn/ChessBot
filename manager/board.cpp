//
// Created by Marvin Becker on 15.12.23.
//

#include "board.h"

Board::Board() : player(WHITE), board{Piece()} {
  readFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

bool Board::popLastMove() {
  if (moves.empty() || history.empty()) {
    return false;
  }

  Move last_move = moves.back();
  moves.pop_back();
  history.pop_back();
  board[last_move.moveSquare] = last_move.capturedPiece;
  board[last_move.square] = last_move.movingPiece;

  if (last_move.moveType == EN_PASSANT) {
    int enPassantSquare =
        last_move.moveSquare + (last_move.movingPiece.isWhite() ? -8 : +8);
    board[enPassantSquare].pieceType = (last_move.movingPiece.isWhite() ? BP : WP);
  }

  if (last_move.moveType == CASTLING) {
    if (last_move.moveSquare == 6) {
      board[7].pieceType = WR;
      board[5].pieceType = EMPTY;
    }
    if (last_move.moveSquare == 2) {
      board[0].pieceType = WR;
      board[3].pieceType = EMPTY;
    }
    if (last_move.moveSquare == 62) {
      board[63].pieceType = BR;
      board[61].pieceType = EMPTY;
    }
    if (last_move.moveSquare == 58) {
      board[56].pieceType = BR;
      board[59].pieceType = EMPTY;
    }
  }

  player = player == WHITE ? BLACK : WHITE;

  boardSettings = history.back();
  return true;
}

void Board::makeMove(Move move) {
  board[move.moveSquare] = board[move.square];
  board[move.square].pieceType = EMPTY;
  boardSettings.epSquare = 100;

  if (move.moveType == PROMOTION) {
    board[move.moveSquare] = move.promotionPiece;
  }

  if (move.moveType == EN_PASSANT) {
    int enPassantSquare = move.moveSquare + (move.movingPiece.pieceType == WP ? -8 : +8);
    board[enPassantSquare].pieceType = EMPTY;
  }

  if (move.moveType == CASTLING) {
    if (move.moveSquare == 6) {
      board[7].pieceType = EMPTY;
      board[5].pieceType = WR;
    }
    if (move.moveSquare == 2) {
      board[0].pieceType = EMPTY;
      board[3].pieceType = WR;
    }
    if (move.moveSquare == 62) {
      board[63].pieceType = EMPTY;
      board[61].pieceType = BR;
    }
    if (move.moveSquare == 58) {
      board[56].pieceType = EMPTY;
      board[59].pieceType = BR;
    }
  }

  if (move.movingPiece.pieceType == WP || move.movingPiece.pieceType == BP) {
    if (std::abs(move.square - move.moveSquare) == 16) {
      boardSettings.epSquare =
          move.moveSquare + (move.movingPiece.isWhite() ? -8 : +8);
    }
  }

  handleCastlingPermissions(move);

  moves.push_back(move);
  history.push_back(boardSettings);
  player = player == WHITE ? BLACK : WHITE;
}

void Board::handleCastlingPermissions(Move &move) {
  if (move.movingPiece.pieceType == WK) {
    boardSettings.whiteQueenSide = false;
    boardSettings.whiteKingSide = false;
  } else if (move.movingPiece.pieceType == BK) {
    boardSettings.blackQueenSide = false;
    boardSettings.blackKingSide = false;
  }

  if (move.movingPiece.pieceType == WR) {
    if (move.square == 0) {
      boardSettings.whiteQueenSide = false;
    }
    if (move.square == 7) {
      boardSettings.whiteKingSide = false;
    }
  } else if (move.movingPiece.pieceType == BR) {
    if (move.square == 56) {
      boardSettings.blackQueenSide = false;
    }

    if (move.square == 63) {
      boardSettings.blackKingSide = false;
    }
  }

  if (move.capturedPiece.pieceType == WR) {
    if (move.moveSquare == 0) {
      boardSettings.whiteQueenSide = false;
    }
    if (move.moveSquare == 7) {
      boardSettings.whiteKingSide = false;
    }
  } else if (move.capturedPiece.pieceType == BR) {
    if (move.moveSquare == 56) {
      boardSettings.blackQueenSide = false;
    }
    if (move.moveSquare == 63) {
      boardSettings.blackKingSide = false;
    }
  }
}

//bool Board::isCheckMate(bool isWhite) {
//  for (int row = 8; row >= 1; row--) {
//    for (int column = 1; column <= 8; column++) {
//      if (isWhite && board[calculatePosition(column, row)].isWhite()) {
//        if (getPossibleMoves(column, row) > 0) {
//          return false;
//        }
//      }
//      if (!isWhite && (!board[calculatePosition(column, row)].isWhite())) {
//        if (getPossibleMoves(column, row) > 0) {
//          return false;
//        }
//      }
//    }
//  }
//  return true;
//}

bool Board::movePiece(char fig, int x, int y, int move_x, int move_y,
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

  Move move =
      generateMove(position, movePosition, findKeyByValue(promotion_figure),
                   promotion ? PROMOTION : NORMAL);
  makeMove(move);

//  if (isKingInCheck(player == WHITE)) {
//    std::cout << "King is in check" << std::endl;
//  } else {
//    std::cout << "No one is in check" << std::endl;
//  }

  return true;
}

bool Board::canMove(char fig, int x, int y, int move_x, int move_y,
                           bool capture) {
  int position = calculatePosition(x, y);
  int move = calculatePosition(move_x, move_y);

  // Check if moveSquare is out of bounds!
  if (position < 0 || position > 63 || move < 0 || move > 63) {
    return false;
  }

  // Check if movingPiece is really that movingPiece.
  if (board[position].toChar() != fig) {
    return false;
  }

  //  if (canCastle(x, y, move_x, move_y)) {
  //    return true;
  //  }

  // Check if capture an empty field or a field without a capture
  if ((board[move].pieceType != EMPTY && !capture) || (capture && board[move].pieceType == EMPTY)) {
    return false;
  }

  if (capture) {
    // Check if you try to capture your own team.
    if ((board[position].isWhite() && board[move].isWhite()) ||
        ((!board[position].isWhite()) && (!board[move].isWhite()))) {
      return false;
    }
  }

  switch (tolower(fig)) {
    case 'p':
      if (isPathClear(x, y, move_x, move_y, board)) {
        return canPawnMove(x, y, move_x, move_y, capture, isupper(fig));
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

bool Board::isPathClear(int startX, int startY, int endX, int endY,
                               const std::array<Piece, 64>& board) {
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
        if (board[calculatePosition(x, y)].pieceType != EMPTY) {
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
        if (board[calculatePosition(x, y)].pieceType != EMPTY) {
          return false;
        }
      }
    }
  }

  return true;
}

bool Board::isWhiteKingInDanger() {
  int whiteKingPositionX = 0;
  int whiteKingPositionY = 0;

  // Get King moveSquare
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      if (board[calculatePosition(x, y)].pieceType == WK) {
        whiteKingPositionX = x;
        whiteKingPositionY = y;
      }
    }
  }

  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      Piece figure = board[calculatePosition(x, y)];
      if (!figure.isWhite()) {
        if (canMove(figure.toChar(), x, y, whiteKingPositionX,
                    whiteKingPositionY, true)) {
          return true;
        }
      }
    }
  }

  return false;
}

bool Board::isBlackKingInDanger() {
  int blackKingPositionX = 0;
  int blackKingPositionY = 0;

  // Get King moveSquare
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      if (board[calculatePosition(x, y)].pieceType == BK) {
        blackKingPositionX = x;
        blackKingPositionY = y;
        break;
      }
    }
  }

  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      Piece figure = board[calculatePosition(x, y)];
      if (figure.isWhite()) {
        if (canMove(figure.toChar(), x, y, blackKingPositionX,
                    blackKingPositionY, true)) {
          return true;
        }
      }
    }
  }

  return false;
}

//bool Board::isKingInCheck(bool isWhite) {
//  if (isWhite && isWhiteKingInDanger()) {
//    return isCheckMate(isWhite);
//  }
//  if (!isWhite && isBlackKingInDanger()) {
//    return isCheckMate(isWhite);
//  }
//  return false;
//}

void Board::readFen(std::string input) {
  std::vector<std::string> fenSettings;

  std::istringstream iss(input);
  for (std::string s; iss >> s;) fenSettings.push_back(s);
  player = WHITE;
  boardSettings = board_setting{100, false, false, false, false};

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
      case 'k':
        board[calculatePosition(x, y)] = Piece(BK);
        x++;
        break;
      case 'K':
        board[calculatePosition(x, y)] = Piece(WK);
        x++;
        break;
      case 'q':
        board[calculatePosition(x, y)] = Piece(BQ);
        x++;
        break;
      case 'Q':
        board[calculatePosition(x, y)] = Piece(WQ);
        x++;
        break;
      case 'r':
        board[calculatePosition(x, y)] = Piece(BR);
        x++;
        break;
      case 'R':
        board[calculatePosition(x, y)] = Piece(WR);
        x++;
        break;
      case 'b':
        board[calculatePosition(x, y)] = Piece(BB);
        x++;
        break;
      case 'B':
        board[calculatePosition(x, y)] = Piece(WB);
        x++;
        break;
      case 'n':
        board[calculatePosition(x, y)] = Piece(BN);
        x++;
        break;
      case 'N':
        board[calculatePosition(x, y)] = Piece(WN);
        x++;
        break;
      case 'p':
        board[calculatePosition(x, y)] = Piece(BP);
        x++;
        break;
      case 'P':
        board[calculatePosition(x, y)] = Piece(WP);
        x++;
        break;
      default:
        for (int i = input[inputIndex] - '0'; i > 0; --i) {
          board[calculatePosition(x, y)] = Piece(EMPTY);
          x++;
        }
        break;
    }
    inputIndex++;
  }

  if (fenSettings[1] == "b") {
    player = BLACK;
  }

  if (fenSettings[2].contains('K')) {
    boardSettings.whiteKingSide = true;
  }
  if (fenSettings[2].contains('Q')) {
    boardSettings.whiteQueenSide = true;
  }
  if (fenSettings[2].contains('k')) {
    boardSettings.blackKingSide = true;
  }
  if (fenSettings[2].contains('q')) {
    boardSettings.blackQueenSide = true;
  }

  if (fenSettings[3] != "-") {
    // TODO POSSIBLE EP
  }

  history.push_back(boardSettings);
  printCurrentBoard();
}

//int Board::getPossibleMoves(int x, int y) {
//  std::array<Piece, 64> saveBoard;
//  saveBoard = board;
//  //  std::memcpy(saveBoard, board, sizeof(board));
//  PieceType piece = board[calculatePosition(x, y)];
//  int possibleMoves = 0;
//  for (int row = 8; row >= 1; row--) {
//    for (int column = 1; column <= 8; column++) {
//      if (board[calculatePosition(column, row)] == EMPTY &&
//          canMove(piece, x, y, column, row, false)) {
//        // Mach den Move!
//        board[calculatePosition(column, row)] = piece;
//        board[calculatePosition(x, y)] = EMPTY;
//        if ((isWhitePiece(piece) && !isWhiteKingInDanger()) ||
//            ((!isWhitePiece(piece)) && !isBlackKingInDanger())) {
//          possibleMoves++;
//        }
//        board = saveBoard;
//        //        std::memcpy(board, saveBoard, sizeof(saveBoard));
//        continue;
//      }
//      if (canMove(piece, x, y, column, row, true)) {
//        // Mach den Move!
//        board[calculatePosition(column, row)] = piece;
//        board[calculatePosition(x, y)] = EMPTY;
//        if ((isWhitePiece(piece) && !isWhiteKingInDanger()) ||
//            ((!isWhitePiece(piece)) && !isBlackKingInDanger())) {
//          possibleMoves++;
//        }
//        board = saveBoard;
//        //        std::memcpy(board, saveBoard, sizeof(saveBoard));
//        continue;
//      }
//    }
//  }
//  return possibleMoves;
//}
//
//void Board::printPossibleMoves(char fig, int x, int y) {
//  std::array<PieceType, 64> saveBoard{};
//  saveBoard = board;
//  //  std::memcpy(saveBoard, board, sizeof(board));
//  PieceType figure = board[calculatePosition(x, y)];
//
//  for (int row = 8; row >= 1; row--) {
//    for (int column = 1; column <= 8; column++) {
//      if (board[calculatePosition(column, row)] == EMPTY &&
//          canMove(fig, x, y, column, row, false)) {
//        // Mach den Move!
//        board[calculatePosition(column, row)] = figure;
//        board[calculatePosition(x, y)] = EMPTY;
//        if ((isupper(fig) && !isWhiteKingInDanger()) ||
//            (islower(fig) && !isBlackKingInDanger())) {
//          std::cout << "[o]";
//          board = saveBoard;
//          //          std::memcpy(board, saveBoard, sizeof(saveBoard));
//          continue;
//        }
//        board = saveBoard;
//        //        std::memcpy(board, saveBoard, sizeof(saveBoard));
//      }
//      if (canMove(fig, x, y, column, row, true)) {
//        // Mach den Move!
//        board[calculatePosition(column, row)] = figure;
//        board[calculatePosition(x, y)] = EMPTY;
//        if ((isupper(fig) && !isWhiteKingInDanger()) ||
//            (islower(fig) && !isBlackKingInDanger())) {
//          std::cout << "[x]";
//          board = saveBoard;
//          //          std::memcpy(board, saveBoard, sizeof(saveBoard));
//          continue;
//        }
//        board = saveBoard;
//        //        std::memcpy(board, saveBoard, sizeof(saveBoard));
//      }
//      std::cout << "[" << pieceToCharMap[board[calculatePosition(x, y)]] << "]";
//      board = saveBoard;
//      //      std::memcpy(board, saveBoard, sizeof(saveBoard));
//    }
//    std::cout << std::endl;
//  }
//  board = saveBoard;
//  //  std::memcpy(board, saveBoard, sizeof(saveBoard));
//}

void Board::printCurrentBoard() {
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
      std::cout << "[" << board[calculatePosition(x, y)].toChar() << "]";
    }
    std::cout << std::endl;
  }
  std::cout << "     a";
  for (int i = 2; i <= 8; i++) {
    std::cout << "  " << char(i + 96);
  }
  std::cout << std::endl;
}

Move Board::generateMove(int position, int moveToPosition,
                                Piece promotionPiece, MoveType moveType) {
  Move move{};
  move.moveSquare = moveToPosition;
  move.square = position;
  move.movingPiece = board[position];
  move.capturedPiece = board[moveToPosition];
  move.promotionPiece = promotionPiece;
  move.moveType = moveType;
  return move;
}
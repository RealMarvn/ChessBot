//
// Created by Marvin Becker on 15.12.23.
//

#include "./board.h"

#include "./chess_bot.h"

bool Board::isKingInCheck(bool pieceColor) {
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x <= 8; x++) {
      if (board[calculateSquare(x, y)].pieceType == (pieceColor ? WK : BK)) {
        return isSquareAttacked({x, y}, pieceColor);
      }
    }
  }

  return false;
}

bool Board::isSquareAttacked(std::pair<int, int> square, bool pieceColor) {
  PseudoLegalMoves allKnightMoves;
  PseudoLegalMoves allPawnMoves;
  PseudoLegalMoves allBishopMoves;
  PseudoLegalMoves allRookMoves;

  // getALlPossible###Moves does only return captures of the opponent pieces. So no need to check again if you capture
  // your own piece

  moveGenUtils::getAllPossibleKnightMoves(square, *this, allKnightMoves, pieceColor);
  for (Move& move : allKnightMoves) {
    if (move.capturedPiece.pieceType == WN || move.capturedPiece.pieceType == BN) {
      return true;
    }
  }

  moveGenUtils::getAllPossiblePawnMoves(square, *this, allPawnMoves, pieceColor);
  for (Move& move : allPawnMoves) {
    if (move.capturedPiece.pieceType == WP || move.capturedPiece.pieceType == BP) {
      return true;
    }
  }

  // You also have to check for Queens because they can move like the bishop too!
  moveGenUtils::getAllPossibleBishopMoves(square, *this, allBishopMoves, pieceColor);
  for (Move& move : allBishopMoves) {
    if (move.capturedPiece.pieceType == WB || move.capturedPiece.pieceType == BB ||
        move.capturedPiece.pieceType == WQ || move.capturedPiece.pieceType == BQ) {
      return true;
    }
  }

  // You also have to check for Queens because they can move like the rook too!
  moveGenUtils::getAllPossibleRookMoves(square, *this, allRookMoves, pieceColor);
  for (Move& move : allRookMoves) {
    if (move.capturedPiece.pieceType == WR || move.capturedPiece.pieceType == BR ||
        move.capturedPiece.pieceType == WQ || move.capturedPiece.pieceType == BQ) {
      return true;
    }
  }

  // Specifically check if there is a king near you.
  std::pair<int, int> directions[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
  for (const auto& dir : directions) {
    int x = square.first + dir.first;
    int y = square.second + dir.second;
    if (x > 0 && y > 0 && x < 9 && y < 9) {
      if (board[calculateSquare(x, y)].pieceType == (pieceColor ? BK : WK)) {
        return true;
      }
    }
  }

  return false;
}

bool Board::popLastMove() {
  // You shouldn't be able to pop if there is nothing.
  if (moves.empty() || history.empty()) {
    return false;
  }

  Move last_move = moves.back();
  moves.pop_back();
  history.pop_back();
  board[last_move.moveSquare] = last_move.capturedPiece;
  board[last_move.square] = last_move.movingPiece;

  if (last_move.moveType == EN_PASSANT) {
    int enPassantSquare = last_move.moveSquare + (last_move.movingPiece.isWhite() ? -8 : +8);
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

  // Settings reset.
  boardSettings = history.back();
  return true;
}

bool Board::makeMove(Move move) {
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
      // Set EP square if a pawn moves exact 2 rows.
      boardSettings.epSquare = move.moveSquare + (move.movingPiece.isWhite() ? -8 : +8);
    }
  }

  // Set the permissions!
  handleCastlingPermissions(move);

  // Save move
  moves.push_back(move);
  // Save settings
  history.push_back(boardSettings);
  player = player == WHITE ? BLACK : WHITE;

  // Check if your king is in check after the move and pop if yes.
  if (isKingInCheck(player != WHITE)) {
    popLastMove();
    return false;
  }
  // return true if everything is fine.
  return true;
}

void Board::handleCastlingPermissions(Move& move) {
  // If king is moved. disable everything.
  if (move.movingPiece.pieceType == WK) {
    boardSettings.whiteQueenSide = false;
    boardSettings.whiteKingSide = false;
  } else if (move.movingPiece.pieceType == BK) {
    boardSettings.blackQueenSide = false;
    boardSettings.blackKingSide = false;
  }

  // disable permission if rook is moved.
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

  // disable permission if rook is captured.
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

bool Board::isCheckMate(bool isWhite) {
  int counter = 0;
  for (Move& move : moveGenUtils::getAllPseudoLegalMoves(*this, isWhite)) {
    if (makeMove(move)) {
      counter++;
      popLastMove();
    }
  }
  return counter == 0;
}

Move Board::parseMove(std::string input) {
  bool capture = false;
  char promotion_figure = ' ';
  char figure = input[0];
  // Use the ascii code of the char to subtract a number to get the correct number.
  int x = input[1] - 96;
  int y = input[2] - 48;
  int position = calculateSquare(x, y);

  int move_x = input[3] - 96;
  int move_y = input[4] - 48;

  // Capture can be lower or upper case.
  if (input[3] == 'x' || input[3] == 'X') {
    move_x = input[4] - 96;
    move_y = input[5] - 48;
    capture = true;
  }
  int movePosition = calculateSquare(move_x, move_y);

  // If there is a capture the promotion is moved.
  if ((capture && input.length() == 8 && input[6] == '=')) {
    promotion_figure = input[7];
  } else if (input.length() == 7 && input[5] == '=') {
    promotion_figure = input[6];
  }
  MoveType moveType = NORMAL;

  // If figure is a king
  if (figure == 'K') {
    if (position == 4 && (movePosition == 6 || movePosition == 2)) {
      moveType = CASTLING;
    }
  }

  if (figure == 'k') {
    if (position == 60 && (movePosition == 62 || movePosition == 58)) {
      moveType = CASTLING;
    }
  }

  if (movePosition == boardSettings.epSquare) {
    moveType = EN_PASSANT;
  }

  if (promotion_figure != ' ') {
    moveType = PROMOTION;
  }

  return Move{movePosition, position, Piece(figure), board[movePosition], Piece(promotion_figure), moveType};
}

bool Board::tryToMovePiece(Move& move) {
  bool capture = false;
  if (move.capturedPiece.pieceType != EMPTY) {
    capture = true;
  }

  // Check if you try to move a piece of the opponent.
  if ((player == BLACK && move.movingPiece.isWhite()) || (player == WHITE && (!move.movingPiece.isWhite()))) {
    return false;
  }

  // Check if moveSquare is out of bounds!
  if (move.square < 0 || move.square > 63 || move.moveSquare < 0 || move.moveSquare > 63) {
    return false;
  }

  // Check if moving piece is really that piece.
  if (board[move.square].pieceType != move.movingPiece.pieceType) {
    return false;
  }

  if (capture) {
    // Check if you try to capture your own team.
    if ((board[move.square].isWhite() && board[move.moveSquare].isWhite()) ||
        ((!board[move.square].isWhite()) && (!board[move.moveSquare].isWhite()))) {
      return false;
    }
  }

  // Check if your move is pseudo legal.
  if (moveGenUtils::getAllPseudoLegalMoves(*this, player == WHITE).contains(move)) {
    // Check moves legality.
    if (!makeMove(move)) {
      std::cout << "Move not legal! Check your king!" << std::endl;
      return false;
    }
    return true;
  } else {
    return false;
  }
}

void Board::readFen(const std::string& input) {
  std::vector<std::string> fenSettings;

  std::istringstream iss(input);
  for (std::string s; iss >> s;) fenSettings.push_back(s);

  if (fenSettings.size() != 6) {
    throw InvalidFENException();
  }

  player = WHITE;
  boardSettings = board_setting{100, false, false, false, false};

  int x = 1;
  int y = 8;
  for (char& character : fenSettings[0]) {
    if (character == '/') {
      y--;
      x = 1;
      continue;
    }

    if (std::isdigit(character)) {
      for (int i = character - '0'; i > 0; --i) {
        board[calculateSquare(x, y)] = Piece(EMPTY);
        x++;
      }
      continue;
    }

    board[calculateSquare(x, y)] = Piece(character);
    x++;
  }

  // Set turn
  if (fenSettings[1] == "b") {
    player = BLACK;
  }

  // Set casteling permissions to true
  if (fenSettings[2].find('K') != std::string::npos) {
    boardSettings.whiteKingSide = true;
  }
  if (fenSettings[2].find('Q') != std::string::npos) {
    boardSettings.whiteQueenSide = true;
  }
  if (fenSettings[2].find('k') != std::string::npos) {
    boardSettings.blackKingSide = true;
  }
  if (fenSettings[2].find('q') != std::string::npos) {
    boardSettings.blackQueenSide = true;
  }

  // Set ep square if given.
  if (fenSettings[3] != "-") {
    int col = fenSettings[3][0] - 96;
    int row = fenSettings[3][1] - 48;
    boardSettings.epSquare = calculateSquare(col, row);
  }

  // Save current settings.
  history.push_back(boardSettings);
  moves.clear();
}

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
      std::cout << "[" << board[calculateSquare(x, y)].toChar() << "]";
    }
    std::cout << std::endl;
  }
  std::cout << "     a";
  for (int i = 2; i <= 8; i++) {
    std::cout << "  " << char(i + 96);
  }
  std::cout << std::endl;
}

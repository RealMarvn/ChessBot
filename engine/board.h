//
// Created by Marvin Becker on 15.12.23.
//

#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "./misc/board_settings.h"
#include "./misc/move.h"
#include "./misc/piece.h"
#include "./misc/player.h"

/**
 * @brief Calculates the square in the board on the x and y coordinates.
 *
 * The position is calculated using the formula: ((y - 1) * 8 + x) - 1.
 * The grid has 8 columns and the indexing starts from 0, so the result is subtracted by 1.
 *
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @return The position in the grid.
 */
inline int calculateSquare(int x, int y) { return ((y - 1) * 8 + x) - 1; }

/**
 * @class Board
 * @brief Represents a chessboard.
 *
 * The Board class represents a chessboard and provides functions for managing the state of the board and making moves.
 */
class Board {
 private:
  // Represents the board.
  std::array<Piece, 64> board;

 public:
  // Represents the current side to move.
  player_type player;
  // Represents all moves so far.
  std::vector<Move> moves;
  // Represents all settings so far.
  std::vector<board_setting> history;
  // Represents the current settings.
  board_setting boardSettings;

  // To access the board at the given index directly.
  Piece& operator[](int index) { return board[index]; }

  /**
   * @brief Initializes a new Board object.
   *
   * This constructor initializes a new Board object. It sets the current player to WHITE and initializes
   * each piece on the board to an empty piece. It then reads the initial board state from a FEN string.
   */
  explicit Board() : player(WHITE), board{Piece()} {
    readFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  };

  /**
   * @brief Checks if the king of the given piece color is in check.
   *
   * This function iterates through the board to find the king of the given piece color.
   * Once the king is found, it calls the isSquareAttacked function to check if the king is being attacked.
   *
   * @param pieceColor The color of the king to check (true for white, false for black).
   * @return True if the king is in check, false otherwise.
   */
  bool isKingInCheck(bool pieceColor);

  /**
   * @brief Checks if the given square is attacked by any piece of the specified opponents color.
   *
   * This function checks if the given square on the chessboard is attacked by any piece of the specified side to moves
   * counterpart (super piece method). It iterates through all possible knight, pawn, bishop, and rook moves and checks
   * if any of them captures their counterpart on the given square. It also checks if the square is attacked by the king
   * in the adjacent positions.
   *
   * @param square The square to check.
   * @param pieceColor The color of the piece to check (true for white, false for black).
   * @return True if the square is attacked, false otherwise.
   */
  bool isSquareAttacked(std::pair<int, int> square, bool pieceColor);

  /**
   * @brief Checks a move and moves a chess piece on the board.
   *
   * This function attempts to move a chess piece on the board. It checks various conditions to ensure the move is valid
   * and updates the board accordingly. The function returns true if the move is successful, and false otherwise.
   * If false the move will not be applied!.
   *
   * @param fig The character representation of the piece to move.
   * @param x The x-coordinate of the current position of the piece.
   * @param y The y-coordinate of the current position of the piece.
   * @param move_x The x-coordinate of the destination position.
   * @param move_y The y-coordinate of the destination position.
   * @param capture True if the move is a capture, false otherwise.
   * @param promotion_figure The character representation of the piece to promote to, if the move is a promotion.
   * @return True if the move is successful, false otherwise. If false the move will not be applied!
   */
  bool movePiece(char fig, int x, int y, int move_x, int move_y, bool capture, char promotion_figure);

  /**
   * @brief Moves a chess piece on the board.
   *
   * This function moves a chess piece on the board. It only checks if the move is legal, you don't set yourself in
   * check, and updates the board accordingly. This function does not check if the move is correct. Use @see movePiece
   * for correct checking. The function returns true if the move is successful, and false otherwise. If false the move
   * will not be applied!
   *
   * @param move The move to make.
   * @return True if the move is successful, false otherwise. If false the move will not be applied!.
   */
  bool makeMove(Move move);

  /**
   * @brief Builds a Move object with the provided parameters.
   *
   * This function constructs a Move object with the provided parameters.
   * It sets the moveSquare, square, movingPiece, capturedPiece, promotionPiece,
   * and moveType fields of the Move object, and returns the constructed Move object.
   *
   * @param position The starting position of the move.
   * @param moveToPosition The destination position of the move.
   * @param promotionPiece The piece to promote to (if the move is a promotion).
   * @param moveType The type of the move.
   * @return The constructed Move object.
   */
  Move buildMove(int position, int moveToPosition, Piece promotionPiece, MoveType moveType);

  /**
   * @brief Removes the last move from the list of moves and updates the board state accordingly.
   *
   * This function removes the last move from the list of moves and updates the board state by reversing the changes
   * made by the move. It returns true if the move was successfully undone, and false otherwise.
   *
   * @return True if the last move was successfully undone, false otherwise.
   */
  bool popLastMove();

  /**
   * @brief Prints the current state of the chessboard.
   *
   * This function prints the current state of the chessboard.
   * It shows the current turn and the positions of all the pieces on the board.
   */
  void printCurrentBoard();

  /**
   * @brief Reads a FEN string and sets up the board accordingly.
   *
   * This function reads a FEN (Forsyth-Edwards Notation) string and sets up the chessboard based on the provided FEN
   * string. Then FEN has to be correct. Otherwise the programm will crash!
   *
   * @param input The FEN string to parse.
   */
  void readFen(std::string input);

  /**
   * @brief Checks if the current player is in checkmate.
   *
   * This function determines if the current player is in checkmate.
   * It generates all possible pseudo-legal moves for the current player and checks if any of them result in a
   * non-checkmate position. If there are no such moves, then checkmate is detected.
   *
   * @param isWhite True if the current player is white, false otherwise.
   * @return True if the current player is in checkmate, false otherwise.
   */
  bool isCheckMate(bool isWhite);

 private:
  /**
   * @brief Updates the castling permissions after a move.
   *
   * This function updates the castling permissions on the board after a move.
   * It checks the type of the moving piece and the target square to determine which castling permissions should be
   * disabled.
   *
   * @param move The move that was made.
   */
  void handleCastlingPermissions(Move& move);
};

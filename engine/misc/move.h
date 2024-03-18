//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <array>
#include <cassert>
#include <sstream>
#include <string>
#include <utility>
#include <algorithm>

#include "./piece.h"

#define MAX_MOVES 218

enum MoveType { NORMAL, EN_PASSANT, PROMOTION, CASTLING };

struct Move {
  int moveSquare{0};
  int square{0};
  Piece movingPiece;
  Piece capturedPiece;
  Piece promotionPiece;
  MoveType moveType{NORMAL};

  /**
   * @brief Converts the Move object's square and moveSquare coordinates to corresponding X and Y coordinates.
   *
   * This function takes the square and moveSquare coordinates of a Move object and converts them to X and Y coordinates.
   * The X coordinate is obtained by adding the ASCII value of 'a' to the remainder of square divided by 8, and casting it to a character.
   * The Y coordinate is obtained by adding 1 to the result of square divided by 8.
   * The same process is applied to the moveSquare coordinates to obtain the second set of X and Y coordinates.
   * The resulting X and Y coordinates are then concatenated together as a string and returned.
   *
   * @return A string representation of the X and Y coordinates of the Move object.
   */
  [[nodiscard]] std::string convertToXandY() const {
    std::ostringstream out;
    out << static_cast<char>((square) % 8 + 'a') << (square) / 8 + 1 << static_cast<char>((moveSquare) % 8 + 'a')
        << (moveSquare) / 8 + 1;
    return out.str();
  }

  /**
   * @brief Overloaded equality operator for comparing two Move objects.
   *
   * @param other The other Move object to compare with.
   * @return True if the two Move objects are equal, false otherwise.
   */
  bool operator==(const Move& other) const {
    return moveSquare == other.moveSquare && square == other.square && movingPiece.pieceType == other.movingPiece.pieceType
           && capturedPiece.pieceType == other.capturedPiece.pieceType && promotionPiece.pieceType == other.promotionPiece.pieceType && moveType == other.moveType;
  }
};

/**
 * @class PseudoLegalMoves
 *
 * @brief Represents a collection of pseudo-legal chess moves.
 *
 * The PseudoLegalMoves class provides a container for storing pseudo-legal chess moves. It uses a fixed-size array
 * to store the moves and provides methods for accessing and modifying the move list.
 */
class PseudoLegalMoves {
  std::array<Move, MAX_MOVES> move_list{};
  int index = 0;

 public:
  using iterator = std::array<Move, MAX_MOVES>::iterator;

  /**
   * @brief Returns an iterator pointing to the first element in the move list.
   *
   * This function returns an iterator pointing to the first element in the move list.
   *
   * @return An iterator pointing to the first element in the move list.
   */
  iterator begin() { return move_list.begin(); }

  /**
   * @fn iterator PseudoLegalMoves::end()
   *
   * @brief Returns an iterator one past the last element of the move list.
   *
   * This function returns an iterator pointing to one past the last element in the move list. It is used to indicate the end
   * of the range of elements in the move list.
   *
   * @return An iterator pointing to one past the last element of the move list.
   */
  iterator end() { return move_list.begin() + index; }

  /**
   * @brief Overloaded subscript operator for accessing a move in the move list.
   *
   * This function allows access to a move in the move list using the subscript operator [].
   * It performs index validation by asserting that the provided number is less than the index.
   * If the number is valid, the corresponding move is returned.
   *
   * @param number The index number of the desired move in the move list.
   * @return A reference to the move at the specified index.
   * @pre The move list must not be empty.
   * @pre Number must be less than the index.
   * @post None.
   */
  Move& operator[](int number) {
    assert(number < index);
    return move_list[number];
  }

  /**
   * @brief Adds a move to the move list.
   *
   * This function adds a move to the move list by assigning it to the element at the current index. It then increments the index by 1.
   *
   * @param mv The move to add to the move list.
   * @pre The index must be less than the maximum number of moves allowed.
   * @post The move is added to the move list and the index is incremented by 1.
   */
  void push_back(const Move& mv) {
    assert(index < MAX_MOVES);
    move_list[index] = mv;
    ++index;
  }

  /**
   * @fn bool PseudoLegalMoves::contains(const Move& mv)
   *
   * @brief Checks if the move list contains a specified move.
   *
   * This function checks if the move list contains the specified move. It uses the std::find algorithm to search for the move in the move list. If the move is found, it returns true;
   * otherwise, it returns false.
   *
   * @param mv The move to check for in the move list.
   * @return True if the move list contains the specified move, false otherwise.
   */
  bool contains(const Move& mv) {
    return std::find(begin(), end(), mv) != end();
  }
};
//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <map>

enum PieceType { WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK, EMPTY };

/**
   * @brief The Piece class represents a chess piece.
   *
   * This class encapsulates the data and behavior of a chess piece.
   * It has a pieceType member variable which stores the type of the piece.
   * The class provides methods to retrieve information about the piece, such as whether it is white,
   * its character representation, and its material value.
   * It also provides static member variables and methods for piece value and character mappings.
 */
class Piece {
 public:
  /**
   * @brief Constructs a new Piece object with the given piece type.
   *
   * @param piece The PieceType representing the piece type.
   */
  explicit Piece(PieceType piece) : pieceType{piece} {};

  /**
   * @brief Constructs a new Piece object with the given piece type.
   *
   * @param piece The character representing the piece type.
   */
  explicit Piece(char piece) : pieceType{findKeyByValue(piece)} {};

  /**
   * @class Piece
   * Represents an EMPTY piece.
   */
  Piece() : pieceType{EMPTY} {};

  PieceType pieceType{EMPTY};

  /**
     * @brief Checks if the piece is white.
     *
     * This function returns true if the piece is white, and false otherwise.
     *
     * @return True if the piece is white, false otherwise.
     */
  [[nodiscard]] inline bool isWhite() const { return (pieceType < BP); };

  /**
   * @brief Returns the character representation of the piece.
   *
   * This function returns the character representation of the piece based on its pieceType.
   *
   * @return The character representation of the piece.
   */
  [[nodiscard]] inline char toChar() const { return pieceToChar[pieceType]; };

  /**
   * @brief Retrieves the material value of the piece.
   *
   * This method calculates and returns the material value of the piece.
   * The material value is based on the type of the piece and whether it is white.
   * If the piece type is EMPTY, the material value is 0.
   * If the piece type is not EMPTY, the material value is calculated as follows:
   * - If the piece is white, the material value is positive.
   * - If the piece is black, the material value is negative.
   * - The material value is multiplied by the pieceValue of the piece type.
   *
   * @return The material value of the piece.
   */
  [[nodiscard]] int getMaterialValue() const {
    if (pieceType == EMPTY) return 0;
    return ((isWhite() ? 1 : -1) * pieceValue[pieceType]);
  };

 private:
  static int pieceValue[6];
  static char pieceToChar[13];
  /**
    * @brief Finds the PieceType based on the given character value.
    *
    * This function searches for the character value in the pieceToChar array
    * and returns the corresponding PieceType.
    * If the character value is not found in the array, it returns EMPTY.
    *
    * @param value The character value representing the piece.
    * @return The PieceType corresponding to the character value.
    */
  PieceType findKeyByValue(char value);
};
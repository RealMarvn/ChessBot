//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <array>
#include <utility>
#include <vector>

#include "../board.h"

namespace moveGenUtils {

const static std::array<PieceType, 4> whitePawnPossiblePromotions = {WQ, WR, WN, WB};
const static std::array<PieceType, 4> blackPawnPossiblePromotions = {BQ, BR, BN, BB};

PseudoLegalMoves getAllPseudoLegalMoves(Board& board, bool player);

void getAllPossibleRookMoves(std::pair<int, int> startSquare, Board& board, PseudoLegalMoves& allPseudoMoves,
                             bool pieceColor);

void getAllPossiblePawnMoves(std::pair<int, int> startSquare, Board& board, PseudoLegalMoves& allPseudoMoves,
                             bool pieceColor);

void getAllPossibleQueenMoves(std::pair<int, int> startSquare, Board& board, PseudoLegalMoves& allPseudoMoves,
                              bool pieceColor);

void getAllPossibleKingMoves(std::pair<int, int> startSquare, Board& board, PseudoLegalMoves& allPseudoMoves,
                             bool pieceColor);

void getAllPossibleKnightMoves(std::pair<int, int> startSquare, Board& board, PseudoLegalMoves& allPseudoMoves,
                               bool pieceColor);

void getAllPossibleBishopMoves(std::pair<int, int> startSquare, Board& board, PseudoLegalMoves& allPseudoMoves,
                               bool pieceColor);
}  // namespace moveGenUtils

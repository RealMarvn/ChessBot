//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <array>
#include <utility>
#include <vector>

#include "./board.h"

const std::array<PieceType, 4> whitePawnPossiblePromotions = {WQ, WR, WN, WB};
const std::array<PieceType, 4> blackPawnPossiblePromotions = {BQ, BR, BN, BB};


bool isKingInCheck(bool pieceColor, Board& boardManager);
bool isSquareAttacked(std::pair<int, int> square, Board& boardManager, bool pieceColor);

AllPseudoMoves getAllPseudoLegalMoves(Board& boardManager,
                                      bool player);

void getAllPossibleRookMoves(std::pair<int, int> startSquare,
                             Board& boardManager,
                             AllPseudoMoves& allPseudoMoves, bool pieceColor);

void getAllPossiblePawnMoves(std::pair<int, int> startSquare,
                             Board& boardManager,
                             AllPseudoMoves& allPseudoMoves, bool pieceColor);

void getAllPossibleQueenMoves(std::pair<int, int> startSquare,
                              Board& boardManager,
                              AllPseudoMoves& allPseudoMoves, bool pieceColor);

void getAllPossibleKingMoves(std::pair<int, int> startSquare,
                             Board& boardManager,
                             AllPseudoMoves& allPseudoMoves, bool pieceColor);

void getAllPossibleKnightMoves(std::pair<int, int> startSquare,
                               Board& boardManager,
                               AllPseudoMoves& allPseudoMoves, bool pieceColor);

void getAllPossibleBishopMoves(std::pair<int, int> startSquare,
                               Board& boardManager,
                               AllPseudoMoves& allPseudoMoves, bool pieceColor);
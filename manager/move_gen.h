//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <array>
#include <utility>
#include <vector>

#include "./misc/move.h"
#include "./misc/piece.h"
#include "./board_manager.h"

const std::array<piece, 4> whitePawnPossiblePromotions = {WQ, WR, WN, WB};
const std::array<piece, 4> blackPawnPossiblePromotions = {BQ, BR, BN, BB};


bool isKingInCheck(bool pieceColor, BoardManager& boardManager);

AllPseudoMoves getAllPseudoLegalMoves(BoardManager& boardManager,
                                      bool player);

void getAllPossibleRookMoves(std::pair<int, int> startSquare,
                             BoardManager& boardManager,
                             AllPseudoMoves& allPseudoMoves);

void getAllPossiblePawnMoves(std::pair<int, int> startSquare,
                             BoardManager& boardManager,
                             AllPseudoMoves& allPseudoMoves);

void getAllPossibleQueenMoves(std::pair<int, int> startSquare,
                              BoardManager& boardManager,
                              AllPseudoMoves& allPseudoMoves);

void getAllPossibleKingMoves(std::pair<int, int> startSquare,
                             BoardManager& boardManager,
                             AllPseudoMoves& allPseudoMoves);

void getAllPossibleKnightMoves(std::pair<int, int> startSquare,
                               BoardManager& boardManager,
                               AllPseudoMoves& allPseudoMoves);

void getAllPossibleBishopMoves(std::pair<int, int> startSquare,
                               BoardManager& boardManager,
                               AllPseudoMoves& allPseudoMoves);
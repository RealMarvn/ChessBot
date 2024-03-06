//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <array>
#include <utility>
#include <vector>

#include "./misc/move.h"
#include "./misc/piece.h"

bool isKingInCheck(bool pieceColor, std::array<piece, 65>& board);

AllPseudoMoves getAllPseudoLegalMoves(std::array<piece, 65>& board,
                                      bool player);

void getAllPossibleRookMoves(std::pair<int, int> startSquare,
                             std::array<piece, 65>& board,
                             AllPseudoMoves& allPseudoMoves);

void getAllPossiblePawnMoves(std::pair<int, int> startSquare,
                             std::array<piece, 65>& board,
                             AllPseudoMoves& allPseudoMoves);

void getAllPossibleQueenMoves(std::pair<int, int> startSquare,
                              std::array<piece, 65>& board,
                              AllPseudoMoves& allPseudoMoves);

void getAllPossibleKingMoves(std::pair<int, int> startSquare,
                             std::array<piece, 65>& board,
                             AllPseudoMoves& allPseudoMoves);

void getAllPossibleKnightMoves(std::pair<int, int> startSquare,
                               std::array<piece, 65>& board,
                               AllPseudoMoves& allPseudoMoves);

void getAllPossibleBishopMoves(std::pair<int, int> startSquare,
                               std::array<piece, 65>& board,
                               AllPseudoMoves& allPseudoMoves);

bool canRookMove(int x, int y, int move_x, int move_y);
bool canQueenMove(int x, int y, int move_x, int move_y);
bool canPawnMove(int x, int y, int move_x, int move_y, bool capture,
                 bool isWhite);
bool canPawnPromote(bool isWhite, char figure, char promotion_figure,
                    int movePosition);
bool canKnightMove(int x, int y, int move_x, int move_y);
bool canKingMove(int x, int y, int move_x, int move_y);
bool canBishopMove(int x, int y, int move_x, int move_y);
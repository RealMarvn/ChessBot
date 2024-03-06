//
// Created by Marvin Becker on 05.03.24.
//

#pragma once
#include <array>
#include <utility>
#include <vector>

#include "./misc/move.h"
#include "./misc/piece.h"

std::vector<Move> getAllPossibleRookMoves(std::pair<int, int> startPos,
                                          piece board[65]);

std::vector<Move> getAllPossiblePawnMoves(std::pair<int, int> startPos,
                                          piece board[65]);

std::vector<Move> getAllPossibleQueenMoves(std::pair<int, int> startPos,
                                           piece board[65]);

std::vector<Move> getAllPossibleKingMoves(std::pair<int, int> startPos,
                                          piece board[65]);

std::vector<Move> getAllPossibleKnightMoves(std::pair<int, int> startPos,
                                            piece board[65]);

std::vector<Move> getAllPossibleBishopMoves(std::pair<int, int> startPos,
                                            piece board[65]);

bool canRookMove(int x, int y, int move_x, int move_y);
bool canQueenMove(int x, int y, int move_x, int move_y);
bool canPawnMove(int x, int y, int move_x, int move_y, bool capture,
                 bool isWhite);
bool canPawnPromote(bool isWhite, char figure, char promotion_figure,
                    int movePosition);
bool canKnightMove(int x, int y, int move_x, int move_y);
bool canKingMove(int x, int y, int move_x, int move_y);
bool canBishopMove(int x, int y, int move_x, int move_y);
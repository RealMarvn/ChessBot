//
// Created by Marvin Becker on 04.01.24.
//

#pragma once

#include "../board_manager.h"

bool canPawnMove(int x, int y, int move_x, int move_y, bool capture,
                 bool isWhite);

bool canPawnPromote(bool isWhite, char figure, char promotion_figure,
                    int movePosition);

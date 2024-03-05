//
// Created by Marvin Becker on 05.03.24.
//

#pragma once

#include "./piece.h"

struct Move {
  int position;
  int old_position;
  piece figure;
  piece capturedFigure;
};

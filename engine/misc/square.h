//
// Created by Marvin Becker on 14.03.24.
//

#pragma once
#include <string>
#include <iostream>
#include <sstream>

class Square {
 private:
  int position;

 public:
  Square(int x, int y) : position{((y - 1) * 8 + x) - 1} {};
  Square(int position) : position{position} {};

  std::string convertToXandY() const {
    std::ostringstream out;
    out << static_cast<char>((position) % 8 + 'a') << (position) / 8 + 1;
    return out.str();
  }

  int getX() const {
    return position % 8;
  }
  int getY() const {
    return (position / 8) + 1;
  }
};

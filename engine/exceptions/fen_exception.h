//
// Created by Marvin Becker on 18.03.24.
//

#pragma once

#include <exception>
#include <string>
#include <utility>

class InvalidFENException : public std::exception {
 public:
  explicit InvalidFENException() = default;
  [[nodiscard]] const char* what() const noexcept override { return "FEN is invalid!!!"; }
};

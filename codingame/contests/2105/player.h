#pragma once

#include <array>
#include <iostream>

class Player {
 public:
  unsigned id;
  unsigned sun;
  unsigned score;
  bool waiting;
  std::array<unsigned, 4> ntrees;

  void Read(unsigned _id) {
    id = _id;
    std::cin >> sun >> score;
    if (id == 0) std::cin >> waiting;
    std::cin.ignore();
  }
};

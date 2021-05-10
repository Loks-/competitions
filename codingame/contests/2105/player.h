#pragma once

#include <array>
#include <iostream>

class Player {
 public:
  uint8_t id;
  unsigned sun;
  unsigned score;
  bool waiting;
  std::array<uint8_t, 4> ntrees;

  void Read(uint8_t _id) {
    id = _id;
    std::cin >> sun >> score;
    if (id == 0)
      std::cin >> waiting;
    else
      waiting = false;
    std::cin.ignore();
  }
};

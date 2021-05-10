#pragma once

#include "common/base.h"
#include "common/vector/read.h"

#include <iostream>
#include <vector>

class Cell {
 public:
  uint8_t index;     // 0 is the center cell, the next cells spiral outwards
  uint8_t richness;  // 0 if the cell is unusable, 1-3 for usable cells
  std::vector<uint8_t> neighbors;  // the index of the neighbouring cell

  void Read() {
    std::cin >> index >> richness;
    neighbors = nvector::Read<uint8_t>(6);
    std::cin.ignore();
  }

  static std::vector<Cell> ReadCells() {
    uint8_t total;
    std::cin >> total;
    std::cin.ignore();
    assert(total == 37);
    std::vector<Cell> cells(total);
    for (auto& c : cells) c.Read();
    return cells;
  }
};

using Cells = std::vector<Cell>;

#pragma once

#include "common/base.h"
#include "common/vector/read.h"

#include <iostream>
#include <vector>

class Cell {
 public:
  unsigned index;     // 0 is the center cell, the next cells spiral outwards
  unsigned richness;  // 0 if the cell is unusable, 1-3 for usable cells
  std::vector<unsigned> neighbors;  // the index of the neighbouring cell

  void Read() {
    std::cin >> index >> richness;
    neighbors = nvector::Read<unsigned>(6);
    std::cin.ignore();
  }

  static std::vector<Cell> ReadCells() {
    unsigned total;
    std::cin >> total;
    std::cin.ignore();
    assert(total == 37);
    std::vector<Cell> cells(total);
    for (auto& c : cells) c.Read();
    return cells;
  }
};

using Cells = std::vector<Cell>;

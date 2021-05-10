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
  std::vector<std::vector<uint8_t>> neighbors_by_distance;

  Cell() {}
  Cell(uint8_t _index, uint8_t _richness,
       const std::vector<uint8_t>& _neighbors)
      : index(_index), richness(_richness), neighbors(_neighbors) {}

  void Read() {
    int _index, _richness;
    std::cin >> _index >> _richness;
    auto vn = nvector::Read<int>(6);
    std::cin.ignore();
    index = _index;
    richness = _richness;
    neighbors.clear();
    for (auto n : vn) neighbors.push_back(uint8_t((n < 0) ? 255 : n));
  }
};

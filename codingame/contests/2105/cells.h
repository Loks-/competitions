#pragma once

#include "cell.h"

#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"

#include <iostream>
#include <vector>

class Cells {
 public:
  std::vector<Cell> cells;

  Cell& operator[](uint8_t index) { return cells[index]; }

  uint8_t Size() const { return cells.size(); }

  void InitNBD() {
    DirectedGraph g(Size());
    for (uint8_t i = 0; i < Size(); ++i) {
      for (auto j : cells[i].neighbors) {
        if (j >= Size()) continue;
        g.AddEdge(i, j);
      }
    }
    for (uint8_t i = 0; i < Size(); ++i) {
      auto v = DistanceFromSource(g, i);
      auto& c = cells[i];
      c.neighbors_by_distance.clear();
      c.neighbors_by_distance.resize(4);
      for (uint8_t j = 0; j < v.size(); ++j) {
        if (v[j] < 4) {
          c.neighbors_by_distance[v[j]].push_back(j);
        }
      }
    }
  }

  void Read() {
    unsigned total;
    std::cin >> total;
    std::cin.ignore();
    assert(total == 37);
    cells.resize(total);
    for (auto& c : cells) c.Read();
    InitNBD();
  }
};

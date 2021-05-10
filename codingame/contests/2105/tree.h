#pragma once

#include <iostream>

class Tree {
 public:
  unsigned cell;  // location of this tree
  unsigned size;  // size of this tree: 0-3
  unsigned player;
  bool used;  // 1 if this tree is dormant (used)

  void Read() {
    std::cin >> cell >> size >> player >> used;
    std::cin.ignore();
  }
};

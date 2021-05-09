#pragma once

#include <iostream>

class Tree {
 public:
  unsigned cell;  // location of this tree
  unsigned size;  // size of this tree: 0-3
  unsigned player;
  bool isDormant;  // 1 if this tree is dormant

  void Read() {
    std::cin >> cell >> size >> player >> isDormant;
    std::cin.ignore();
  }
};

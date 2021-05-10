#pragma once

#include <iostream>

class Tree {
 public:
  uint8_t cell;  // location of this tree
  uint8_t size;  // size of this tree: 0-3
  uint8_t player;
  bool used;  // 1 if this tree is dormant (used)

  void Read() {
    int _cell, _size, _player;
    std::cin >> _cell >> _size >> _player >> used;
    std::cin.ignore();
    cell = _cell;
    size = _size;
    player = _player;
  }
};
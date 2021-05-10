#pragma once

#include "player.h"
#include "tree.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

class Position {
 public:
  uint8_t day;
  uint8_t nutrients;
  std::array<Player, 2> players;
  std::vector<Tree> trees;

  Player& Me() { return players[1]; }
  Player& Opponent() { return players[0]; }

  void CountTrees() {
    for (auto& p : players) std::fill(p.ntrees.begin(), p.ntrees.end(), 0u);
    for (auto& t : trees) players[t.player].ntrees[t.size] += 1;
  }

  void Read() {
    unsigned _day, _nutrients;
    std::cin >> _day;
    std::cin.ignore();
    std::cin >> _nutrients;
    std::cin.ignore();
    day = _day;
    nutrients = _nutrients;
    players[1].Read(1);
    players[0].Read(0);
    unsigned ntrees;
    std::cin >> ntrees;
    std::cin.ignore();
    trees.resize(ntrees);
    for (auto& t : trees) t.Read();
    unsigned npactions;
    std::cin >> npactions;
    std::cin.ignore();
    std::string stemp;
    for (unsigned i = 0; i < npactions; ++i) std::getline(std::cin, stemp);
    CountTrees();
  }
};

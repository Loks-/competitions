#pragma once

#include "player.h"
#include "tree.h"

#include <array>
#include <iostream>
#include <string>
#include <vector>

class GameTurn {
 public:
  unsigned day;
  unsigned nutrients;
  std::array<Player, 2> players;
  std::vector<Tree> trees;

  Player& Me() { return players[1]; }
  Player& Opponent() { return players[0]; }

  void Read() {
    std::cin >> day;
    std::cin.ignore();
    std::cin >> nutrients;
    std::cin.ignore();
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
  }
};

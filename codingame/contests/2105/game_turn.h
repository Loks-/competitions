#pragma once

#include "action.h"
#include "player.h"
#include "tree.h"

#include <algorithm>
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

  unsigned GrowBaseCost(unsigned new_size) const {
    return (1u << new_size) - 1;
  }

  unsigned CompleteCost() const { return 4; }

  Player& Me() { return players[1]; }
  Player& Opponent() { return players[0]; }

  void CountTrees() {
    for (auto& p : players) std::fill(p.ntrees.begin(), p.ntrees.end(), 0u);
    for (auto& t : trees) players[t.player].ntrees[t.size] += 1;
  }

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
    CountTrees();
  }

  Actions GetPossibleActions(unsigned player) const {
    Actions r;
    auto& p = players[player];
    r.push_back(Action(WAIT));
    if (p.waiting) return r;
    for (auto& t : trees) {
      if (t.player != player) continue;
      // SEED
      // ...
      if ((t.size < 3) &&
          (p.sun >= p.ntrees[t.size + 1] + GrowBaseCost(t.size + 1)))
        r.push_back(Action(GROW, t.cell));
      if ((t.size == 3) && (p.sun >= CompleteCost()))
        r.push_back(Action(COMPLETE, t.cell));
    }
    return r;
  }
};

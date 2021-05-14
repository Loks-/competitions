#pragma once

#include "game.h"

#include <vector>

class Featurization {
 public:
  void Apply(const Game& game, unsigned player, std::vector<double>& output) {
    auto& p = game.pos;
    auto &p0 = p.players[player ^ 1], &p1 = p.players[player];
    output.clear();
    output.push_back(int(p1.score) - int(p0.score));
    for (unsigned ip = 0; ip < 2; ++ip) {
      auto& pi = p.players[ip ^ player];
      output.push_back(pi.sun);
      output.push_back(pi.waiting);
      for (unsigned j = 0; j < 4; ++j) {
        output.push_back(pi.ntrees[j]);
      }
    }
  }
};

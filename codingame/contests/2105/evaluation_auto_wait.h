#pragma once

#include "game.h"
#include "settings.h"

#include "common/base.h"

#include <vector>

inline int64_t EvaluationAutoWait(const Game& game) {
  thread_local std::vector<uint8_t> shade;
  thread_local std::vector<unsigned> ps(2);
  ps[0] = game.pos.players[0].sun;
  ps[1] = game.pos.players[1].sun;
  for (unsigned day = game.pos.day + 1; day < TotalDays(); ++day) {
    game.CalcShades(day, shade);
    for (auto& t : game.pos.trees) {
      if (t.size > shade[t.cell]) ps[t.player] += t.size;
    }
  }
  return int64_t(game.pos.players[1].score + ps[1] / 3) -
         int64_t(game.pos.players[0].score + ps[0] / 3);
}

#pragma once

#include "evaluation.h"
#include "game.h"
#include "settings.h"

#include "common/base.h"

#include <vector>

class EvaluationAutoWait : public Evaluation {
 protected:
  std::vector<uint8_t> shade;
  std::vector<unsigned> ps;

 public:
  EvaluationAutoWait() : ps(2) {}

  int64_t Apply(const Game& game) {
    ps[0] = game.pos.players[0].sun;
    ps[1] = game.pos.players[1].sun;
    for (unsigned day = game.pos.day + 1; day < TotalDays(); ++day) {
      game.CalcShades(day, shade);
      for (auto& t : game.pos.trees) {
        if (t.size > shade[t.cell]) ps[t.player] += t.size;
      }
    }
    return PDScore(FinalScore(game.pos.players[0].score, ps[0]),
                   FinalScore(game.pos.players[1].score, ps[1]),
                   Evaluation::player);
  }
};

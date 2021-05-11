#pragma once

#include "game.h"
#include "settings.h"
#include "common/base.h"

#include <array>
#include <vector>

// Each player complete on final day if pmask is set for him.
inline int64_t EvaluationWaitAndComplete(const Game& game,
                                         unsigned pmask_for_complete) {
  thread_local std::vector<uint8_t> shade;
  thread_local std::array<unsigned, 2> psun, pscore;
  for (unsigned i = 0; i < 2; ++i) {
    psun[i] = game.pos.players[i].sun;
    pscore[i] = game.pos.players[i].score;
  }
  for (unsigned day = game.pos.day + 1; day < TotalDays(); ++day) {
    game.CalcShades(day, shade);
    for (auto& t : game.pos.trees) {
      if (t.size > shade[t.cell]) psun[t.player] += t.size;
    }
  }
  if (pmask_for_complete) {
    unsigned nutrients = game.pos.nutrients;
    thread_local std::array<std::vector<unsigned>, 2> vt;
    vt[0].clear();
    vt[1].clear();
    for (auto& c : game.cells.cells) {
      auto cti = game.pos.cell_to_tree[c.index];
      if (cti == 255) continue;
      auto& t = game.pos.trees[cti];
      assert(t.cell == c.index);
      if (t.size == 3) vt[t.player].push_back(c.index);
    }
    for (unsigned i = 0; i < 2; ++i) {
      if (((1u << i) & pmask_for_complete)) {
        if (vt[i].size() > psun[i] / CompleteCost())
          vt[i].resize(psun[i] / CompleteCost());
      } else {
        vt[i].clear();
      }
    }
    for (unsigned i = 0; i < 2; ++i) {
      for (unsigned j = 0; j < vt[i].size(); ++j) {
        unsigned nspent = j + std::min<unsigned>(j, vt[i ^ 1].size()),
                 nadj = (nutrients > nspent) ? nutrients - nspent : 0,
                 cscore = nadj + CompletePointsFromRichness(
                                     game.cells[vt[i][j]].richness);
        if (cscore < 2) break;
        pscore[i] += cscore;
        psun[i] -= CompleteCost();
      }
    }
  }
  return int64_t(pscore[1] + psun[1] / 3) - int64_t(pscore[0] + psun[0] / 3);
}

inline int64_t EvaluationWaitAndComplete(const Game& game,
                                         unsigned pmask_for_complete,
                                         unsigned player) {
  return (2 * int(player) - 1) *
         EvaluationWaitAndComplete(game, pmask_for_complete);
}

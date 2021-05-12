#pragma once

#include "cells.h"
#include "evaluation.h"
#include "game.h"
#include "settings.h"

#include "common/base.h"

#include <array>
#include <vector>

class EvaluationAutoWaitAndComplete : public Evaluation {
 public:
  using TBase = Evaluation;

 protected:
  unsigned pmask;
  std::vector<uint8_t> shade;
  std::vector<unsigned> psun;
  std::vector<unsigned> pscore;
  std::array<std::vector<unsigned>, 2> vt;

 public:
  EvaluationAutoWaitAndComplete() : psun(2), pscore(2) {}

  void Reset(const Cells& cells, unsigned _player) {
    TBase::Reset(cells, _player);
    pmask = (1u << _player);
  }

  int64_t Apply(const Game& game) {
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
    if (pmask) {
      unsigned nutrients = game.pos.nutrients;
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
        if (((1u << i) & pmask)) {
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
    return PDScore(FinalScore(pscore[0], psun[0]),
                   FinalScore(pscore[1], psun[1]), Evaluation::player);
  }
};

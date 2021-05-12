#pragma once

#include "action.h"
#include "evaluation_wait_and_complete.h"
#include "fstrategy_random.h"
#include "game.h"
#include "settings.h"
#include "strategy.h"
#include "wsgc.h"

#include "common/base.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

class StrategyWSGCUTS : public Strategy {
 public:
  class Node {
   public:
    int64_t best_score = -1000;

    void Update(int64_t new_score) {
      best_score = std::max(best_score, new_score);
    }
  };

  class MasterNode {
   public:
    unsigned games = 0;
    wsgc::WSGC<Node> data;
    int64_t evaluation = 0;
  };

 public:
  EvaluationAutoWaitAndComplete e;
  unsigned max_time_per_move_milliseconds = 50;
  bool first_move;
  Game g;
  std::unordered_map<size_t, MasterNode> mnodes;
  unsigned total_runs;

 protected:
  void Apply(Action a) {
    if (Strategy::player)
      g.ApplyActions(Action(AUTO_WAIT), a);
    else
      g.ApplyActions(a, Action(AUTO_WAIT));
  }

  int64_t Play() {
    if (g.pos.day >= TotalDays()) return g.PScore(Strategy::player);
    auto& mnode = mnodes[g.pos.Hash()];
    mnode.games += 1;
    if (mnode.games == 1) {
      // First time, use evaluation instead of search
      mnode.data.Init(g.pos, g.GetPossibleActions(Strategy::player));
      auto r = e.Apply(g);
      if (g.pos.day + 1u < TotalDays())
        mnode.data.Update(Action(WAIT), mnode.evaluation);
      return r;
    }
    auto a = mnode.data.GetAction();
    Apply(a);
    auto r = Play();
    mnode.data.Update(a, r);
    return mnode.data.s.best_score;
  }

 public:
  void Reset(const Cells& cells) override {
    e.Reset(cells, Strategy::player);
    first_move = true;
    mnodes.clear();
    total_runs = 0;
  }

  std::string Name() const override { return "WSGC_UTS"; }

  Action GetAction(const Game& game) override {
    auto t0 = std::chrono::high_resolution_clock::now();
    if (first_move) {
      g.cells = game.cells;
      first_move = false;
    }
    unsigned runs = 0;
    for (; std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now() - t0)
               .count() < max_time_per_move_milliseconds;
         ++runs) {
      g.pos = game.pos;
      Play();
    }
    total_runs += runs;
    auto p = Strategy::player;
    auto& mnode = mnodes[game.pos.Hash()];
    // std::cerr << "Total games = " << mnode.games << "\tRuns = " << runs
    //           << "\tTotal = " << total_runs
    //           << "\tBest score = " << mnode.data.s.best_score << std::endl;
    if (mnode.games < 1) return FStrategyRandom::Get(game, p);
    return mnode.data.GetBestAction(
        [](auto& l, auto& r) { return l.best_score > r.best_score; });
  }

  static PStrategy Make() { return std::make_shared<StrategyWSGCUTS>(); }
};

#pragma once

#include "action.h"
#include "evaluation_proxy.h"
#include "game.h"
#include "settings.h"
#include "strategy.h"
#include "wsgc.h"

#include "common/base.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

template <class TFStrategy0, class TFStrategy1 = TFStrategy0>
class StrategyWSGCUTS2 : public Strategy {
 public:
  class Node {
   public:
    int64_t best_score = -MCMaxScore();

    void Update(int64_t new_score) {
      best_score = std::max(best_score, new_score);
    }
  };

  class MasterNode {
   public:
    Action action_opp;
    unsigned games = 0;
    WSGC<Node> data;
  };

 public:
  EvaluationProxy<TFStrategy0, TFStrategy1> e;
  Game g;
  std::unordered_map<size_t, MasterNode> mnodes;
  unsigned total_runs;

 protected:
  Action FSActionMe() {
    return Strategy::player ? TFStrategy1::Get(g, 1) : TFStrategy0::Get(g, 0);
  }

  Action FSActionOpp() {
    return Strategy::player ? TFStrategy0::Get(g, 0) : TFStrategy1::Get(g, 1);
  }

  void Apply(Action a_me, Action a_opp) {
    if (Strategy::player)
      g.ApplyActions(a_opp, a_me);
    else
      g.ApplyActions(a_me, a_opp);
  }

  int64_t Play() {
    if (g.pos.day >= TotalDays()) return g.PScoreExt(Strategy::player);
    // auto& mnode = mnodes[g.pos.Hash()];
    size_t h = g.pos.Hash();
    auto& mnode = mnodes[h];
    mnode.games += 1;
    if (mnode.games == 1) {
      // First time
      mnode.action_opp = FSActionOpp();
      mnode.data.Init(g.pos, g.GetPossibleActions(Strategy::player));
      auto r = e.Apply(g);
      mnode.data.Update(FSActionMe(), r);
      return r;
    }
    auto a = mnode.data.GetNextAction();
    Apply(a, mnode.action_opp);
    auto r = Play();
    auto& mnode2 = mnodes[h];
    mnode2.data.Update(a, r);
    return mnode2.data.s.best_score;
  }

 public:
  void Reset(const Cells& cells) override {
    e.Reset(cells, Strategy::player);
    g.cells = cells;
    mnodes.clear();
    total_runs = 0;
  }

  std::string Name() const override { return "WSGC_UTS2"; }

  Action GetAction(const Game& game) override {
    auto t0 = std::chrono::high_resolution_clock::now();
    unsigned runs = 0;
    for (; std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now() - t0)
               .count() < MaxTimePerMove();
         ++runs) {
      g.pos = game.pos;
      Play();
    }
    total_runs += runs;
    auto& mnode = mnodes[game.pos.Hash()];
    // std::cerr << "Total games = " << mnode.games << "\tRuns = " << runs
    //           << "\tTotal = " << total_runs
    //           << "\tBest score = " << mnode.data.s.best_score << std::endl;
    // std::cerr << "W = " << mnode.data.w.s.best_score
    //           << "\tS = " << mnode.data.e.s.best_score
    //           << "\tG = " << mnode.data.g.s.best_score
    //           << "\tC = " << mnode.data.c.s.best_score << std::endl;
    return mnode.data.GetBestAction(
        [](auto& l, auto& r) { return l.best_score > r.best_score; });
  }

  static PStrategy Make() { return std::make_shared<StrategyWSGCUTS2>(); }
};

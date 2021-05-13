#pragma once

#include "action.h"
#include "game.h"
#include "settings.h"
#include "strategy_eproxy.h"
#include "wsgc.h"

#include "common/base.h"

#include <algorithm>
#include <array>
#include <string>
#include <unordered_map>
#include <vector>

template <class TFStrategy0, class TFStrategy1 = TFStrategy0>
class StrategyWSGCUTS2 : public StrategyEProxy<TFStrategy0, TFStrategy1> {
 public:
  using TBase = StrategyEProxy<TFStrategy0, TFStrategy1>;

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

 protected:
  std::unordered_map<size_t, MasterNode> mnodes;
  unsigned total_runs;

 protected:
  int64_t Play() {
    auto& g = TBase::g;
    if (g.Ended()) return g.PScoreExt(TBase::player);
    // auto& mnode = mnodes[g.pos.Hash()];
    size_t h = g.pos.Hash();
    auto& mnode = mnodes[h];
    mnode.games += 1;
    if (mnode.games == 1) {
      // First time
      mnode.action_opp = TBase::FSActionOpp();
      mnode.data.Init(g.pos, g.GetPossibleActions(TBase::player));
      auto r = TBase::e.Apply(g);
      mnode.data.Update(TBase::FSActionMe(), r);
      return r;
    }
    auto a = mnode.data.GetNextAction();
    TBase::Apply(a, mnode.action_opp);
    auto r = Play();
    auto& mnode2 = mnodes[h];
    mnode2.data.Update(a, r);
    return mnode2.data.s.best_score;
  }

 public:
  void Reset(const Cells& cells) override {
    TBase::Reset(cells);
    mnodes.clear();
    total_runs = 0;
  }

  std::string Name() const override { return "WSGC_UTS2"; }

  Action GetAction(const Game& game) override {
    TBase::StartTurn();
    unsigned runs = 0;
    for (; !TBase::TimeToStop(); ++runs) {
      TBase::g.pos = game.pos;
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
    return mnode.data.GetBestAction();
  }

  static PStrategy Make() { return std::make_shared<StrategyWSGCUTS2>(); }
};

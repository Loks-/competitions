#pragma once

#include "action.h"
#include "game.h"
#include "settings.h"
#include "settings_mcts.h"
#include "strategy_eproxy.h"
#include "wsgc.h"

#include "common/base.h"

#include <algorithm>
#include <array>
#include <string>
#include <unordered_map>
#include <vector>

template <class TFStrategy0, class TFStrategy1 = TFStrategy0>
class StrategyWSGCMCTS : public StrategyEProxy<TFStrategy0, TFStrategy1> {
 public:
  using TBase = StrategyEProxy<TFStrategy0, TFStrategy1>;

  class Node {
   public:
    unsigned games = 0;
    int64_t best_score = -MCMaxScore();

    void Update(int64_t new_score) {
      games += 1;
      best_score = std::max(best_score, new_score);
    }

    double L2G() const { return Log2Games(games); }

    double Eval(double l2g) const { return MCScoreExt(best_score, l2g, games); }
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

  int64_t Play(bool first = false) {
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
      auto a = TBase::FSActionMe();
      auto r = TBase::e.ApplyI(g);
      mnode.data.Update(a, r);
      return r;
    } else if (mnode.data.Size() == 1) {
      TBase::Apply(mnode.data.GetWaitAction(), mnode.action_opp);
      return Play();
    }
    auto a = first ? mnode.data.GetMCActionE(mnode.data.GetBestAction())
                   : mnode.data.GetMCAction();
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

  std::string Name() const override { return "WSGC_MCTS"; }

  Action GetAction(const Game& game) override {
    TBase::StartTurn();
    unsigned runs = 0;
    for (; !TBase::TimeToStop(); ++runs) {
      TBase::g.pos = game.pos;
      Play(true);
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

  static PStrategy Make() { return std::make_shared<StrategyWSGCMCTS>(); }
};

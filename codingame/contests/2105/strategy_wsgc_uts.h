#pragma once

#include "action.h"
#include "evaluation_wait_and_complete.h"
#include "fstrategy_random.h"
#include "game.h"
#include "settings.h"
#include "settings_mcts.h"
#include "strategy_time.h"
#include "wsgc.h"

#include "common/base.h"

#include <algorithm>
#include <array>
#include <string>
#include <unordered_map>
#include <vector>

class StrategyWSGCUTS : public StrategyTime {
 public:
  using TBase = StrategyTime;

  class Node {
   public:
    int64_t best_score = -MCMaxScore();

    void Update(int64_t new_score) {
      best_score = std::max(best_score, new_score);
    }
  };

  class MasterNode {
   public:
    unsigned games = 0;
    WSGC<Node> data;
    int64_t evaluation = 0;
  };

 protected:
  EvaluationAutoWaitAndComplete e;
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
    if (g.Ended()) return g.PScore(Strategy::player);
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
    auto a = mnode.data.GetNextAction();
    Apply(a);
    auto r = Play();
    mnode.data.Update(a, r);
    return mnode.data.s.best_score;
  }

 public:
  void Reset(const Cells& cells) override {
    e.Reset(cells, Strategy::player);
    g.cells = cells;
    mnodes.clear();
    total_runs = 0;
  }

  std::string Name() const override { return "WSGC_UTS"; }

  Action GetAction(const Game& game) override {
    TBase::StartTurn();
    unsigned runs = 0;
    for (; !TBase::TimeToStop(); ++runs) {
      g.pos = game.pos;
      Play();
    }
    total_runs += runs;
    auto p = TBase::player;
    auto& mnode = mnodes[game.pos.Hash()];
    // std::cerr << "Total games = " << mnode.games << "\tRuns = " << runs
    //           << "\tTotal = " << total_runs
    //           << "\tBest score = " << mnode.data.s.best_score << std::endl;
    // std::cerr << "W = " << mnode.data.w.s.best_score
    //           << "\tS = " << mnode.data.e.s.best_score
    //           << "\tG = " << mnode.data.g.s.best_score
    //           << "\tC = " << mnode.data.c.s.best_score << std::endl;
    if (mnode.games < 1) return FStrategyRandom::Get(game, p);
    return mnode.data.GetBestAction();
  }

  static PStrategy Make() { return std::make_shared<StrategyWSGCUTS>(); }
};

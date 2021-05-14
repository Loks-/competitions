#pragma once

#include "action.h"
#include "game.h"
#include "settings.h"
#include "settings_mcts.h"
#include "strategy_eproxy.h"

#include "common/base.h"

#include <algorithm>
#include <array>
#include <string>
#include <unordered_map>
#include <vector>

template <class TFStrategy0, class TFStrategy1 = TFStrategy0>
class StrategyMCTS3 : public StrategyEProxy<TFStrategy0, TFStrategy1> {
 public:
  using TBase = StrategyEProxy<TFStrategy0, TFStrategy1>;

  class Node {
   public:
    unsigned games = 0;
    int64_t best_score = -MCMaxScore();

    void Update(int64_t score) {
      games += 1;
      best_score = std::max(best_score, score);
    }
  };

  class MasterNode {
   public:
    Action action_opp;
    unsigned games = 0;
    std::vector<std::pair<Node, Action>> nodes;
    int64_t best_score = -MCMaxScore();
    Action best_action = Action(WAIT);
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
      mnode.best_action = TBase::FSActionMe();
      mnode.best_score = TBase::e.ApplyC(g);
      auto v = g.GetPossibleActions(TBase::player);
      std::random_shuffle(v.begin(), v.end());
      mnode.nodes.resize(v.size());
      for (unsigned j = 0; j < v.size(); ++j) {
        mnode.nodes[j].second = v[j];
        if (v[j] == mnode.best_action)
          mnode.nodes[j].first.Update(mnode.best_score);
      }
      return mnode.best_score;
    } else if (mnode.nodes.size() == 1) {
      TBase::Apply(mnode.best_action, mnode.action_opp);
      return Play();
    } else {
      double best_score = -MCMaxScore(), l2g = Log2Games(mnode.games);
      unsigned best_node = 0;
      for (unsigned j = 0; j < mnode.nodes.size(); ++j) {
        // It's already main candidate
        if (mnode.nodes[j].second == mnode.best_action) continue;
        auto& n = mnode.nodes[j].first;
        if (n.games == 0) {
          best_node = j;
          break;
        }
        double score = MCScoreExt(n.best_score, l2g, n.games);
        if (score > best_score) {
          best_score = score;
          best_node = j;
        }
      }
      TBase::Apply(mnode.nodes[best_node].second, mnode.action_opp);
      auto r = Play();
      auto& mnode2 = mnodes[h];
      mnode2.nodes[best_node].first.Update(r);
      if (mnode2.best_score < r) {
        mnode2.best_score = r;
        mnode2.best_action = mnode2.nodes[best_node].second;
      }
      return mnode2.best_score;
    }
  }

 public:
  void Reset(const Cells& cells) override {
    TBase::Reset(cells);
    mnodes.clear();
    total_runs = 0;
  }

  std::string Name() const override { return "MCTS3"; }

  Action GetAction(const Game& game) override {
    TBase::StartTurn();
    unsigned runs = 0;
    for (; !TBase::TimeToStop(); ++runs) {
      // for (; runs < 100; ++runs) {
      TBase::g.pos = game.pos;
      Play();
    }
    total_runs += runs;
    auto& mnode = mnodes[game.pos.Hash()];
    // std::cerr << Name() << " " << mnode.best_score << std::endl;
    // std::cerr << "Total games = " << mnode.games << "\tRuns = " << runs
    //           << "\tTotal = " << total_runs << "\tSize = " << mnodes.size()
    //           << std::endl;
    return mnode.best_action;
  }

  static PStrategy Make() { return std::make_shared<StrategyMCTS3>(); }
};

#pragma once

#include "action.h"
#include "evaluation_proxy.h"
#include "game.h"
#include "settings.h"
#include "settings_mcts.h"
#include "strategy.h"

#include "common/base.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

template <class TFStrategy0, class TFStrategy1 = TFStrategy0>
class StrategyMCTS3 : public Strategy {
 public:
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
      mnode.best_action = FSActionMe();
      mnode.best_score = e.Apply(g);
      auto v = g.GetPossibleActions(Strategy::player);
      std::random_shuffle(v.begin(), v.end());
      mnode.nodes.resize(v.size());
      for (unsigned j = 0; j < v.size(); ++j) {
        mnode.nodes[j].second = v[j];
        if (v[j] == mnode.best_action)
          mnode.nodes[j].first.Update(mnode.best_score);
      }
      return mnode.best_score;
    } else if (mnode.nodes.size() == 1) {
      Apply(mnode.best_action, mnode.action_opp);
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
      Apply(mnode.nodes[best_node].second, mnode.action_opp);
      auto r = Play();
      auto& mnode2 = mnodes[h];  // mnode reference can be wrong!
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
    e.Reset(cells, Strategy::player);
    g.cells = cells;
    mnodes.clear();
    total_runs = 0;
  }

  std::string Name() const override { return "MCTS3"; }

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
    //           << "\tTotal = " << total_runs << std::endl;
    return mnode.best_action;
  }

  static PStrategy Make() { return std::make_shared<StrategyMCTS3>(); }
};

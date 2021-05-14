#pragma once

#include "action.h"
#include "evaluation_wait_and_complete.h"
#include "fstrategy_random.h"
#include "game.h"
#include "settings.h"
#include "settings_mcts.h"
#include "strategy.h"

#include "common/base.h"

#include <algorithm>
#include <array>
#include <string>
#include <unordered_map>
#include <vector>

class StrategyMCTS2 : public StrategyTime {
 public:
  using TBase = StrategyTime;

  class Node {
   public:
    unsigned games = 0;
    int64_t total_score = 0;
  };

  class MasterNode {
   public:
    unsigned games = 0;
    std::vector<std::vector<std::pair<Node, Action>>> nodes;
    int64_t evaluation = 0;
  };

 protected:
  EvaluationAutoWaitAndComplete e;
  bool one_side_seach = true;
  Game g;
  std::unordered_map<size_t, MasterNode> mnodes;
  unsigned total_runs;

 protected:
  unsigned EPMask() const {
    return one_side_seach ? (1u << Strategy::player) : 3;
  }

  int64_t Play() {
    if (g.Ended()) return g.PScore(1);
    auto& mnode = mnodes[g.pos.Hash()];
    mnode.games += 1;
    if (mnode.games == 1) {
      // First time, use evaluation instead of search
      return (mnode.evaluation = e.ApplyI(g));
    }
    if (mnode.nodes.empty()) {
      assert(mnode.games == 2);
      mnode.nodes.resize(2);
      thread_local std::array<std::vector<Action>, 2> pactions;
      pactions[0] = g.GetPossibleActions(0);
      pactions[1] = g.GetPossibleActions(1);
      for (unsigned i = 0; i < 2; ++i) {
        assert(!pactions[i].empty() && pactions[i][0].IsWait());
        std::random_shuffle(pactions[i].begin() + 1, pactions[i].end());
        mnode.nodes[i].resize(pactions[i].size());
        for (unsigned j = 0; j < pactions[i].size(); ++j)
          mnode.nodes[i][j].second = pactions[i][j];
        auto& wnode = mnode.nodes[i][0].first;
        wnode.games += 1;
        wnode.total_score += mnode.evaluation;
      }
    }
    std::array<unsigned, 2> pm{0, 0};
    for (unsigned i = 0; i < 2; ++i) {
      if (one_side_seach && (i != Strategy::player)) continue;
      double best_score = -MCMaxScore(), l2g = Log2Games(mnode.games);
      auto& nodes = mnode.nodes[i];
      for (unsigned j = 0; j < nodes.size(); ++j) {
        auto& n = nodes[j].first;
        if (n.games == 0) {
          pm[i] = j;
          break;
        }
        double score =
            PMult(i) *
            MCScore(double(n.total_score) / double(n.games), l2g, n.games);
        if (score > best_score) {
          best_score = score;
          pm[i] = j;
        }
      }
    }
    g.ApplyActions(mnode.nodes[0][pm[0]].second, mnode.nodes[1][pm[1]].second);
    auto r = Play();
    for (unsigned i = 0; i < 2; ++i) {
      auto& n = mnode.nodes[i][pm[i]].first;
      n.games += 1;
      n.total_score += r;
    }
    return r;
  }

 public:
  void Reset(const Cells& cells) override {
    e.Reset(cells, Strategy::player);
    g.cells = cells;
    mnodes.clear();
    total_runs = 0;
  }

  std::string Name() const override { return "MCTS2"; }

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
    //           << "\tTotal = " << total_runs << std::endl;
    if (mnode.nodes.size() != 2) return FStrategyRandom::Get(game, p);
    auto& nodes = mnode.nodes[p];
    unsigned move = 0;
    double best_score = -MCMaxScore();
    for (unsigned j = 0; j < nodes.size(); ++j) {
      auto& n = nodes[j].first;
      double score = PMult(p) * double(n.total_score) / std::max(n.games, 1u);
      if (score > best_score) {
        best_score = score;
        move = j;
      }
    }
    return nodes[move].second;
  }

  static PStrategy Make() { return std::make_shared<StrategyMCTS2>(); }
};

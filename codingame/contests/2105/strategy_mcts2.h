#pragma once

#include "action.h"
#include "evaluation_wait_and_complete.h"
#include "fstrategy_random.h"
#include "game.h"
#include "settings.h"
#include "strategy.h"

#include "common/base.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

class StrategyMCTS2 : public Strategy {
 public:
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

 public:
  EvaluationAutoWaitAndComplete e;
  unsigned max_time_per_move_milliseconds = 10;
  double exploration_mult = 14;
  bool one_side_seach = true;
  Game g;
  std::unordered_map<size_t, MasterNode> mnodes;
  unsigned total_runs;

 protected:
  unsigned EPMask() const {
    return one_side_seach ? (1u << Strategy::player) : 3;
  }

  int64_t Play() {
    if (g.pos.day >= TotalDays()) return g.PScore(1);
    auto& mnode = mnodes[g.pos.Hash()];
    mnode.games += 1;
    if (mnode.games == 1) {
      // First time, use evaluation instead of search
      return (mnode.evaluation = e.Apply(g));
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
      double best_score = -1000;
      auto& nodes = mnode.nodes[i];
      for (unsigned j = 0; j < nodes.size(); ++j) {
        auto& n = nodes[j].first;
        if (n.games == 0) {
          pm[i] = j;
          break;
        }
        double score = n.total_score * (2.0 * i - 1.0) / n.games +
                       exploration_mult * sqrt(log2(mnode.games) / n.games);
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
    auto t0 = std::chrono::high_resolution_clock::now();
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
    //           << "\tTotal = " << total_runs << std::endl;
    if (mnode.nodes.size() != 2) return FStrategyRandom::Get(game, p);
    auto& nodes = mnode.nodes[p];
    unsigned move = 0;
    double best_score = -1000;
    for (unsigned j = 0; j < nodes.size(); ++j) {
      auto& n = nodes[j].first;
      double score = n.total_score * (2.0 * p - 1.0) / std::max(n.games, 1u);
      if (score > best_score) {
        best_score = score;
        move = j;
      }
    }
    return nodes[move].second;
  }

  static PStrategy Make() { return std::make_shared<StrategyMCTS2>(); }
};

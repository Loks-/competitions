#pragma once

#include "action.h"
#include "fstrategy_random.h"
#include "game.h"
#include "settings.h"
#include "settings_mcts.h"
#include "strategy.h"

#include "common/base.h"

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

class StrategyMCTS : public Strategy {
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
  };

 public:
  Game g;
  std::unordered_map<size_t, MasterNode> mnodes;
  unsigned total_runs;

 protected:
  int64_t Play() {
    if (g.pos.day >= TotalDays()) return g.PScore(1);
    auto& mnode = mnodes[g.pos.Hash()];
    thread_local std::vector<std::vector<Action>> pactions(2);
    pactions[0] = g.GetPossibleActions(0);
    pactions[1] = g.GetPossibleActions(1);
    // Assume that if hash and number of moves match, everything is safe to go
    if (mnode.nodes.size() == 2) {
      if ((mnode.nodes[0].size() != pactions[0].size()) ||
          (mnode.nodes[1].size() != pactions[1].size()))
        return 0;
    } else {
      mnode.nodes.resize(2);
      for (unsigned i = 0; i < 2; ++i) {
        mnode.nodes[i].resize(pactions[i].size());
        for (unsigned j = 0; j < pactions[i].size(); ++j)
          mnode.nodes[i][j].second = pactions[i][j];
      }
    }
    std::vector<unsigned> pm(2, 0);
    for (unsigned i = 0; i < 2; ++i) {
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
    mnode.games += 1;
    for (unsigned i = 0; i < 2; ++i) {
      auto& n = mnode.nodes[i][pm[i]].first;
      n.games += 1;
      n.total_score += r;
    }
    return r;
  }

 public:
  void Reset(const Cells& cells) override {
    g.cells = cells;
    mnodes.clear();
    total_runs = 0;
  }

  std::string Name() const override { return "MCTS"; }

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
    auto p = Strategy::player;
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

  static PStrategy Make() { return std::make_shared<StrategyMCTS>(); }
};

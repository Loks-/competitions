#pragma once

#include "cell.h"
#include "game.h"
#include "settings.h"
#include "strategy.h"

#include <chrono>
#include <iostream>
#include <random>

class Runner {
 public:
  std::minstd_rand rng;
  Game game;
  PStrategy s0, s1;

  Runner() {
    SetSeed(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
  }

  Runner(unsigned seed) { SetSeed(seed); }

  void SetSeed(unsigned seed) {
    std::cout << "Seed = " << seed << std::endl;
    rng.seed(seed);
  }

  void SetStrategy0(PStrategy s) { s0 = s; }
  void SetStrategy1(PStrategy s) { s1 = s; }

  unsigned PairCell(unsigned index) const {
    if (index == 0) return index;
    if (index < 4) return index + 3;
    if (index < 7) return index - 3;
    if (index < 13) return index + 6;
    if (index < 19) return index - 6;
    if (index < 28) return index + 9;
    if (index < 37) return index - 9;
    assert(false);
    return index;
  }

  void ResetCells() {
    auto& cells = game.cells.cells;
    cells.clear();
    cells.push_back(Cell(0, 3, {1, 2, 3, 4, 5, 6}));
    cells.push_back(Cell(1, 3, {7, 8, 2, 0, 6, 18}));
    cells.push_back(Cell(2, 3, {8, 9, 10, 3, 0, 1}));
    cells.push_back(Cell(3, 3, {2, 10, 11, 12, 4, 0}));
    cells.push_back(Cell(4, 3, {0, 3, 12, 13, 14, 5}));
    cells.push_back(Cell(5, 3, {6, 0, 4, 14, 15, 16}));
    cells.push_back(Cell(6, 3, {18, 1, 0, 5, 16, 17}));
    cells.push_back(Cell(7, 2, {19, 20, 8, 1, 18, 36}));
    cells.push_back(Cell(8, 2, {20, 21, 9, 2, 1, 7}));
    cells.push_back(Cell(9, 2, {21, 22, 23, 10, 2, 8}));
    cells.push_back(Cell(10, 2, {9, 23, 24, 11, 3, 2}));
    cells.push_back(Cell(11, 2, {10, 24, 25, 26, 12, 3}));
    cells.push_back(Cell(12, 2, {3, 11, 26, 27, 13, 4}));
    cells.push_back(Cell(13, 2, {4, 12, 27, 28, 29, 14}));
    cells.push_back(Cell(14, 2, {5, 4, 13, 29, 30, 15}));
    cells.push_back(Cell(15, 2, {16, 5, 14, 30, 31, 32}));
    cells.push_back(Cell(16, 2, {17, 6, 5, 15, 32, 33}));
    cells.push_back(Cell(17, 2, {35, 18, 6, 16, 33, 34}));
    cells.push_back(Cell(18, 2, {36, 7, 1, 6, 17, 35}));
    cells.push_back(Cell(19, 1, {255, 255, 20, 7, 36, 255}));
    cells.push_back(Cell(20, 1, {255, 255, 21, 8, 7, 19}));
    cells.push_back(Cell(21, 1, {255, 255, 22, 9, 8, 20}));
    cells.push_back(Cell(22, 1, {255, 255, 255, 23, 9, 21}));
    cells.push_back(Cell(23, 1, {22, 255, 255, 24, 10, 9}));
    cells.push_back(Cell(24, 1, {23, 255, 255, 25, 11, 10}));
    cells.push_back(Cell(25, 1, {24, 255, 255, 255, 26, 11}));
    cells.push_back(Cell(26, 1, {11, 25, 255, 255, 27, 12}));
    cells.push_back(Cell(27, 1, {12, 26, 255, 255, 28, 13}));
    cells.push_back(Cell(28, 1, {13, 27, 255, 255, 255, 29}));
    cells.push_back(Cell(29, 1, {14, 13, 28, 255, 255, 30}));
    cells.push_back(Cell(30, 1, {15, 14, 29, 255, 255, 31}));
    cells.push_back(Cell(31, 1, {32, 15, 30, 255, 255, 255}));
    cells.push_back(Cell(32, 1, {33, 16, 15, 31, 255, 255}));
    cells.push_back(Cell(33, 1, {34, 17, 16, 32, 255, 255}));
    cells.push_back(Cell(34, 1, {255, 35, 17, 33, 255, 255}));
    cells.push_back(Cell(35, 1, {255, 36, 18, 17, 34, 255}));
    cells.push_back(Cell(36, 1, {255, 19, 7, 18, 35, 255}));
  }

  void DisableRandomCells() {
    std::uniform_int_distribution<> uid(1, 36), uid2(0, 1);
    for (unsigned i = 0; i < 4; ++i) {
      if (uid2(rng)) {
        unsigned cid = uid(rng);
        game.cells[cid].richness = 0;
        game.cells[PairCell(cid)].richness = 0;
      }
    }
  }

  void AddStartingTrees() {
    std::uniform_int_distribution<> uid(19, 36);
    for (; game.pos.trees.size() < 4;) {
      unsigned cid = uid(rng);
      if (game.cells[cid].richness == 0) continue;
      if (game.pos.cell_to_tree[cid] != 255) continue;
      game.pos.AddTree({uint8_t(cid), 1, 0, false});
      game.pos.AddTree({uint8_t(PairCell(cid)), 1, 1, false});
    }
  }

  void Reset() {
    ResetCells();
    DisableRandomCells();
    game.cells.InitNBD();
    game.pos.Reset();
    AddStartingTrees();
    game.pos.CountTrees();
    s0->Reset();
    s1->Reset();
    s0->SetP(0);
    s1->SetP(1);
  }

  void RunGame() {
    Reset();
    game.NextDay(true);
    auto& p0 = game.pos.players[0];
    auto& p1 = game.pos.players[1];
    for (; game.pos.day < TotalDays();) {
      Action a0 = p0.waiting ? Action() : s0->GetAction(game);
      Action a1 = p1.waiting ? Action() : s1->GetAction(game);
      game.ApplyActions(a0, a1);
    }
    std::cout << "S0: [" << p0.score << ", " << p0.sun << ", "
              << p0.CombinedScore() << "] -- " << s0->Name() << std::endl;
    std::cout << "S1: [" << p1.score << ", " << p1.sun << ", "
              << p1.CombinedScore() << "] -- " << s1->Name() << std::endl;
  }
};

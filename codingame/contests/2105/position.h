#pragma once

#include "player.h"
#include "settings.h"
#include "tree.h"

#include "common/base.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

class Position {
 public:
  uint8_t day;
  uint8_t nutrients;
  std::array<Player, 2> players;
  std::vector<Tree> trees;
  std::vector<uint8_t> cell_to_tree;

  Position() : cell_to_tree(CellsSize()) {}

  Player& Me() { return players[1]; }
  Player& Opponent() { return players[0]; }

  void Reset() {
    day = 0;
    nutrients = InitialNutrient();
    trees.clear();
    players[0].Reset(0);
    players[1].Reset(1);
    std::fill(cell_to_tree.begin(), cell_to_tree.end(), 255);
  }

  void DecreaseNutrients() {
    if (nutrients > 0) nutrients -= 1;
  }

  void CountTrees() {
    for (auto& p : players) std::fill(p.ntrees.begin(), p.ntrees.end(), 0u);
    for (auto& t : trees) players[t.player].ntrees[t.size] += 1;
  }

  Tree& GetTreeByCell(uint8_t cell) {
    uint8_t tid = cell_to_tree[cell];
    assert(tid < trees.size());
    return trees[tid];
  }

  void AddTree(const Tree& tree) {
    assert(cell_to_tree[tree.cell] == 255);
    cell_to_tree[tree.cell] = trees.size();
    trees.push_back(tree);
  }

  void RemoveTree(uint8_t cell) {
    uint8_t tid = cell_to_tree[cell];
    assert(tid < trees.size());
    if (tid < trees.size() - 1) {
      trees[tid] = trees.back();
      cell_to_tree[trees[tid].cell] = tid;
    }
    cell_to_tree[cell] = 255;
    trees.pop_back();
  }

  void Read() {
    unsigned _day, _nutrients;
    std::cin >> _day;
    std::cin.ignore();
    std::cin >> _nutrients;
    std::cin.ignore();
    day = _day;
    nutrients = _nutrients;
    players[1].Read(1);
    players[0].Read(0);
    std::fill(cell_to_tree.begin(), cell_to_tree.end(), 255);
    unsigned ntrees;
    std::cin >> ntrees;
    std::cin.ignore();
    trees.resize(ntrees);
    for (unsigned i = 0; i < ntrees; ++i) {
      trees[i].Read();
      cell_to_tree[trees[i].cell] = i;
    }
    unsigned npactions;
    std::cin >> npactions;
    std::cin.ignore();
    std::string stemp;
    for (unsigned i = 0; i < npactions; ++i) std::getline(std::cin, stemp);
    CountTrees();
  }
};

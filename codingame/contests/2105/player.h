#pragma once

#include "settings.h"

#include "common/base.h"
#include "common/hash.h"

#include <array>
#include <iostream>
#include <numeric>

class Player {
 public:
  uint8_t id;
  unsigned sun;
  unsigned score;
  bool waiting;
  std::array<uint8_t, 4> ntrees;

  void Reset(uint8_t _id) {
    id = _id;
    sun = 0;
    score = 0;
    waiting = false;
  }

  void Read(uint8_t _id) {
    id = _id;
    std::cin >> sun >> score;
    if (id == 0)
      std::cin >> waiting;
    else
      waiting = false;
    std::cin.ignore();
  }

  unsigned Trees() const {
    return std::accumulate(ntrees.begin(), ntrees.end(), 0u);
  }

  unsigned HTrees() const {
    unsigned s = 0;
    for (unsigned i = 1; i < 4; ++i) s += i * ntrees[i];
    return s;
  }

  unsigned FScore() const { return FinalScore(score, sun); }

  uint64_t FScoreExt() const {
    return (uint64_t(FScore()) << 14) + (Trees() << 8) + HTrees() + sun;
  }

  size_t Hash() const {
    size_t h = 0;
    h = HashCombine(h, sun);
    h = HashCombine(h, score);
    h = HashCombine(h, waiting);
    return h;
  }
};

#pragma once

#include "mask.h"

#include "common/hash.h"

#include <algorithm>
#include <array>
#include <iostream>

class Position {
 public:
  TMask p0, p1;
  unsigned nmoves;
  std::array<unsigned, WIDTH> vheights;
  std::array<unsigned, SIZE> vmoves;

  Position() {
    p0 = p1 = 0;
    nmoves = 0;
    std::fill(vheights.begin(), vheights.end(), 0u);
  }

  size_t Hash() const { return HashCombine(p0, p1); }

  bool operator==(const Position& r) const {
    return (r.p0 == p0) && (r.p1 == p1);
  }

  TMask& GetMask() { return ((nmoves & 1) ? p1 : p0); }

  bool LegalMove(unsigned c) const { return vheights[c] < HEIGHT; }

  void MakeMove(unsigned c) {
    ((nmoves & 1) ? p1 : p0) ^= (mask_1 << (c * HEIGHT + vheights[c]++));
    vmoves[nmoves++] = c;
  }

  void RevertMove() {
    unsigned c = vmoves[--nmoves];
    ((nmoves & 1) ? p1 : p0) ^= (mask_1 << (c * HEIGHT + --vheights[c]));
  }

  void Print() {
    for (unsigned i = 0; i < nmoves; ++i) std::cout << (vmoves[i] + 1);
  }
};

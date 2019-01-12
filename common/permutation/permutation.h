#pragma once

#include "../base.h"
#include "../vector/read.h"
#include <algorithm>
#include <utility>
#include <vector>

class Permutation {
 public:
  using TLine = std::vector<unsigned>;
  using TCycle = std::vector<unsigned>;
  using TCycles = std::vector<TCycle>;
  using TSwap = std::pair<unsigned, unsigned>;
  using TSwaps = std::vector<TSwap>;

 protected:
  unsigned n;
  mutable TLine line;
  mutable TCycles cycles;
  mutable bool enabled_line;
  mutable bool enabled_cycles;

 protected:
  void ResetLine() const {
    for (unsigned i = 0; i < n; ++i) line[i] = i;
  }

  void EnableLine() const {
    if (enabled_line) return;
    assert(enabled_cycles);
    ResetLine();
    for (const TCycle& c : cycles) {
      for (unsigned i = 1; i < c.size(); ++i) line[c[i - 1]] = c[i];
      line[c.back()] = c[0];
    }
    enabled_line = true;
  }

  void ResetCycles() const { cycles.clear(); }

  void EnableCycles() const {
    if (enabled_cycles) return;
    assert(enabled_line);
    ResetCycles();
    thread_local TLine t;
    t.resize(n);
    std::fill(t.begin(), t.end(), 0);
    for (unsigned i = 0; i < n; ++i) {
      TCycle c;
      for (unsigned j = i; t[j] == 0; j = line[j]) {
        t[j] = 1;
        c.push_back(j);
      }
      if (c.size() > 1) cycles.push_back(c);
    }
    enabled_cycles = true;
  }

 public:
  Permutation(unsigned size = 0) {
    n = size;
    line.resize(n);
    Reset();
  }

  Permutation(const TLine& l, bool shited_line) { SetLine(l, shited_line); }

  Permutation(unsigned size, const TCycles& cs) {
    SetSize(size);
    SetCycles(cs);
  }

  void SetSize(unsigned size) {
    n = size;
    line.resize(n);
    Reset();
  }

  unsigned Size() const { return n; }

  void Reset() {
    ResetLine();
    ResetCycles();
    enabled_line = true;
    enabled_cycles = true;
  }

  void SetLine(const TLine& l, bool shited_line) {
    SetSize(l.size());
    line = l;
    if (shited_line) {
      for (unsigned& j : line) --j;
    }
    enabled_line = true;
    enabled_cycles = false;
  }

  const TLine& Line() const {
    EnableLine();
    return line;
  }

  void SetCycles(const TCycles& cs) {
    cycles = cs;
    enabled_line = false;
    enabled_cycles = true;
  }

  const TCycles& Cycles() const {
    EnableCycles();
    return cycles;
  }

  void ReadAsLine(unsigned n, bool shited_line) {
    TLine l = ReadVector<unsigned>(n);
    SetLine(l, shited_line);
  }
};

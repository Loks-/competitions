#pragma once

#include "common/base.h"
#include <stack>
#include <unordered_map>

// Number of elements in Collatz sequence
namespace numeric {
class Collatz {
 protected:
  std::unordered_map<uint64_t, unsigned> vs;

 public:
  Collatz() { vs[0] = vs[1] = 1; }

  static uint64_t Next(uint64_t n) { return (n & 1) ? (3 * n + 1) : n / 2; }

  unsigned Get(uint64_t n) {
    thread_local std::stack<uint64_t> s;
    auto it = vs.find(n);
    for (; it == vs.end(); it = vs.find(n)) {
      s.push(n);
      n = Next(n);
    }
    unsigned l = it->second;
    for (; !s.empty(); s.pop()) {
      n = s.top();
      vs[n] = ++l;
    }
    return l;
  }

  uint64_t operator()(uint64_t n) { return Get(n); }
};
}  // namespace numeric

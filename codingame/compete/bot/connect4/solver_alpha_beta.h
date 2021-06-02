#pragma once

#include "position.h"
#include "result.h"

#include "common/base.h"
#include "common/numeric/bits/ulog2.h"
#include "common/stl/hash/pair.h"

#include <iostream>
#include <unordered_map>
#include <utility>

class SolverAlphaBeta {
 protected:
  Position p;
  uint64_t ncalls;
  unsigned print_moves;
  std::unordered_map<std::pair<TMask, TMask>, Result> cache;

 public:
  void Reset() {
    ncalls = 0;
    print_moves = p.nmoves + 3;
  }

  Result SolveAB(Result alpha, Result beta) {
    if (p.nmoves == SIZE) return Result::Draw;
    auto& r = cache[std::make_pair(p.p0, p.p1)];
    if (Finalized(r)) return r;
    ++ncalls;
    TMask& mask = p.GetMask();
    for (unsigned c = 0; c < WIDTH; ++c) {
      if (p.LegalMove(c)) {
        p.MakeMove(c);
        bool b = Has4(mask);
        p.RevertMove();
        if (b) return (r = Result::Win);
      }
    }
    Result inv_alpha = Invert(alpha), inv_beta = Invert(beta);
    for (unsigned c = 0; c < WIDTH; ++c) {
      if (p.LegalMove(c)) {
        p.MakeMove(c);
        auto r = SolveAB(inv_beta, inv_alpha);
        p.RevertMove();
        if (r < inv_alpha) {
          inv_alpha = r;
          alpha = Invert(inv_alpha);
          if (alpha >= beta) break;
        }
      }
    }
    if (p.nmoves <= print_moves) {
      p.Print();
      std::cout << " " << unsigned(alpha) << " " << unsigned(beta) << " "
                << numeric::ULog2(ncalls) << " " << numeric::ULog2(cache.size())
                << std::endl;
    }
    return (r = alpha);
  }

  Result Solve(const Position& _p) {
    p = _p;
    Reset();
    return SolveAB(Result::Loss, Result::Win);
  }
};

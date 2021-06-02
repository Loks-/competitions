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
  std::unordered_map<std::pair<TMask, TMask>, unsigned> cache;

 public:
  void Reset() {
    ncalls = 0;
    print_moves = p.nmoves + 3;
  }

  unsigned SolveAB(unsigned alpha, unsigned beta) {
    if (p.nmoves == SIZE) return Result::Draw;
    auto& r = cache[std::make_pair(p.p0, p.p1)];
    if (Result::Finalized(r)) return r;
    switch (r) {
      case Result::Loss:
      case Result::Draw:
      case Result::Win:
        return r;
      case Result::LossOrDraw:
        if ((beta = Result::Draw) <= alpha) return r;
        break;
      case Result::WinOrDraw:
        if ((alpha = Result::Draw) >= beta) return r;
      default:
        break;
    }
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
    unsigned inv_alpha = Result::Invert(alpha), inv_beta = Result::Invert(beta);
    for (unsigned c = 0; c < WIDTH; ++c) {
      if (p.LegalMove(c)) {
        p.MakeMove(c);
        auto t = SolveAB(inv_beta, inv_alpha);
        p.RevertMove();
        if (t < inv_alpha) {
          inv_alpha = t;
          if (inv_alpha <= inv_beta) break;
        }
      }
    }
    unsigned r2 = Result::Invert(inv_alpha);
    if (p.nmoves <= print_moves) {
      p.Print();
      std::cout << " " << r2 << " " << alpha << " " << beta << " "
                << numeric::ULog2(ncalls) << " " << numeric::ULog2(cache.size())
                << std::endl;
    }
    if (r2 == Result::Draw) {
      if (beta == Result::Draw)
        r2 = Result::WinOrDraw;
      else if (alpha == Result::Draw)
        r2 = Result::LossOrDraw;
    }
    // LossOrDraw && WinOrDraw
    if (r == Result::Invert(r2)) r2 = Result::Draw;
    return (r = r2);
  }

  unsigned Solve(const Position& _p) {
    p = _p;
    Reset();
    return SolveAB(Result::Loss, Result::Win);
  }
};

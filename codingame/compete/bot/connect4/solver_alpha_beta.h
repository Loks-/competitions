#pragma once

#include "position.h"
#include "result.h"

#include "common/base.h"
#include "common/numeric/bits/ulog2.h"

#include <iostream>

class SolverAlphaBeta {
 protected:
  Position p;
  uint64_t ncalls;
  unsigned print_moves;

 public:
  void Reset() {
    ncalls = 0;
    print_moves = p.nmoves + 10;
  }

  Result SolveAB(Result alpha, Result beta) {
    ++ncalls;
    if (p.nmoves == SIZE) return Result::Draw;
    TMask& mask = p.GetMask();
    for (unsigned c = 0; c < WIDTH; ++c) {
      if (p.LegalMove(c)) {
        p.MakeMove(c);
        bool b = Has4(mask);
        p.RevertMove();
        if (b) return Result::Win;
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
                << numeric::ULog2(ncalls) << std::endl;
    }
    return alpha;
  }

  Result Solve(const Position& _p) {
    p = _p;
    Reset();
    return SolveAB(Result::Loss, Result::Win);
  }
};

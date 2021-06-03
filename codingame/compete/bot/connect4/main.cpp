#include "board.h"
#include "mask.h"
#include "position.h"
#include "solver_alpha_beta.h"

#include <iostream>
#include <string>

int main(int nargs, char** pargs) {
  Position p;
  // std::string sgame = "45461667"; // R = 5, T = 10s, W = 24
  // std::string sgame = "35333571"; // R = 1, T = 3m 43s, W = 28
  // std::string sgame = "13333111"; // R = 3, T = 8m 49s, W = 29
  std::string sgame = "45461667";
  if (nargs > 1) sgame = pargs[1];
  for (char c : sgame) p.MakeMove(unsigned(c) - unsigned('1'));
  SolverAlphaBeta s;
  auto r = s.Solve(p);
  std::cout << r << std::endl;
  return 0;
}

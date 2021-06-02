#include "board.h"
#include "mask.h"
#include "position.h"
#include "solver_alpha_beta.h"

#include <iostream>
#include <string>

int main(int nargs, char** pargs) {
  Position p;
  // std::string sgame = "45461667";
  // std::string sgame = "35333571";
  // std::string sgame = "13333111";
  std::string sgame = "35333571155";
  if (nargs > 1) sgame = pargs[1];
  for (char c : sgame) p.MakeMove(unsigned(c) - unsigned('1'));
  SolverAlphaBeta s;
  auto r = s.Solve(p);
  std::cout << unsigned(r) << std::endl;
  return 0;
}

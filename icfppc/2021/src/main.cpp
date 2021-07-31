#include "solvers/mctp.h"
#include "utils/evaluate_solution.h"

#include "common/solvers/ext/run_n.h"

int main() {
  //   EvaluateSolution("best");
  src_solvers::MCTP s(10);
  solvers::ext::RunN<src_solvers::MCTP>(s, 1, 132);
  return 0;
}

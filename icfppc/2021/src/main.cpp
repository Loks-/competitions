#include "solvers/mctp.h"
#include "utils/evaluate_solution.h"

#include "common/solvers/ext/run_n.h"

int main() {
  // EvaluateSolution("crbm");
  src_solvers::MCTP s(10);
  solvers::ext::RunN<src_solvers::MCTP>(s, 2, 2);
  return 0;
}

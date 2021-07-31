#pragma once

#include "evaluator.h"
#include "problem.h"
#include "solution.h"

#include "common/solvers/solver.h"

namespace src_solvers {
using Base = solvers::Solver<Problem, Solution, Evaluator>;
}  // namespace src_solvers

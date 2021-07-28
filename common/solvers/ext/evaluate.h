#pragma once

#include <string>

namespace solvers {
namespace ext {
template <class TEvaluator, class TProblem, class TSolution>
inline typename TEvaluator::Result Evaluate(
    const std::string& id, const std::string& problem_filename,
    const std::string& solution_filename) {
  using TResult = typename TEvaluator::Result;
  TProblem p;
  if (!p.Load(id, problem_filename)) return TResult(false, -1);
  TSolution s;
  if (!s.Load(id, solution_filename)) return TResult(false, -2);
  return TEvaluator::Apply(p, s);
}

template <class TEvaluator, class TProblem, class TSolution>
inline typename TEvaluator::Result Evaluate(
    const std::string& problem_filename, const std::string& solution_filename) {
  return Evaluate<TEvaluator, TProblem, TSolution>("", problem_filename,
                                                   solution_filename);
}
}  // namespace ext
}  // namespace solvers
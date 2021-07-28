#pragma once

#include <string>

namespace solvers {
namespace ext {
template <class TEvaluator, class TProblem, class TSolution>
inline typename TEvaluator::Result Evaluate(const std::string& id,
                                            const std::string& solver_name) {
  using TResult = typename TEvaluator::Result;
  TProblem p;
  if (!p.Load(id)) return TResult(false, -1);
  TSolution s;
  if (!s.Load(id, solver_name)) return TResult(false, -2);
  return TEvaluator::Apply(p, s);
}
}  // namespace ext
}  // namespace solvers

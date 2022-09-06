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

template <class TEvaluator, class TProblem, class TSolution>
inline bool UpdateBest(const std::string& id, const std::string& solver_name,
                       const std::string& best_name) {
  TProblem p;
  if (!p.Load(id)) return false;
  TSolution s;
  if (!s.Load(id, solver_name)) return false;
  auto r = TEvaluator::Apply(p, s);
  if (!r.correct) return false;
  TSolution sbest;
  if (!sbest.Load(id, best_name)) {
    s.Save(best_name);
    return true;
  }
  auto rbest = TEvaluator::Apply(p, sbest);
  if (TEvaluator::Compare(r, rbest)) {
    s.Save(best_name);
    return true;
  }
  return false;
}
}  // namespace ext
}  // namespace solvers

#pragma once

#include "common/base.h"
#include "common/solvers/ext/run_one.h"

#include <string>

namespace solvers {
namespace ext {
template <class TSolver>
inline void RunOneThreadSafe(const typename TSolver::PSolver& psolver,
                             const std::string& problem_id) {
  assert(psolver);
  auto ptemp = psolver->Clone();
  assert(ptemp);
  RunOne<TSolver>(*ptemp, problem_id);
}
}  // namespace ext
}  // namespace solvers

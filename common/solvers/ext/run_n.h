#pragma once

#include "run_one_thread_safe.h"

#include <string>

namespace solvers {
namespace ext {
template <class TSolver>
inline void RunNTS(const typename TSolver::PSolver& psolver,
                   unsigned first_problem, unsigned last_problem) {
  for (unsigned i = first_problem; i <= last_problem; ++i) {
    RunOneThreadSafe<TSolver>(psolver, std::to_string(i));
  }
}

template <class TSolver>
inline void RunN(TSolver& s, unsigned first_problem, unsigned last_problem) {
  for (unsigned i = first_problem; i <= last_problem; ++i) {
    RunOne<TSolver>(s, std::to_string(i));
  }
}
}  // namespace ext
}  // namespace solvers

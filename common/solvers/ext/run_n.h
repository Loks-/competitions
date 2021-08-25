#pragma once

#include "common/solvers/ext/run_one.h"
#include "common/solvers/ext/run_one_thread_safe.h"
#include "common/thread_pool.h"

#include <string>

namespace solvers {
namespace ext {
// Single thread version
template <class TSolver>
inline void RunN(TSolver& s, unsigned first_problem, unsigned last_problem) {
  for (unsigned i = first_problem; i <= last_problem; ++i)
    RunOne<TSolver>(s, std::to_string(i));
}

// Multi threads version
template <class TSolver>
inline void RunNMT(TSolver& s, unsigned first_problem, unsigned last_problem,
                   unsigned nthreads) {
  auto psolver = s.Clone();
  {
    // ThreadPool desctructor should be called before destructor for psolver.
    ThreadPool tp(nthreads);
    for (unsigned i = first_problem; i <= last_problem; ++i) {
      auto t = std::make_shared<std::packaged_task<void()>>(
          [&, i]() { RunOneThreadSafe<TSolver>(psolver, std::to_string(i)); });
      tp.EnqueueTask(std::move(t));
    }
  }
}
}  // namespace ext
}  // namespace solvers

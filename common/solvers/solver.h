#pragma once

#include <memory>
#include <string>

namespace solvers {
template <class TTProblem, class TTSolution, class TTEvaluator>
class Solver {
 public:
  using TProblem = TTProblem;
  using TSolution = TTSolution;
  using TEvaluator = TTEvaluator;
  using TSelf = Solver<TProblem, TSolution, TEvaluator>;
  using PSolver = std::shared_ptr<TSelf>;

 protected:
  unsigned max_time_in_seconds;

 public:
  Solver() : max_time_in_seconds(-1u) {}
  explicit Solver(unsigned _max_time) : max_time_in_seconds(_max_time) {}

  virtual ~Solver() {}
  virtual PSolver Clone() const { return nullptr; }

  virtual bool SkipSolutionRead() const { return false; }
  virtual bool SkipSolutionWrite() const { return false; }
  virtual bool SkipBest() const { return false; }

  virtual std::string Name() const { return ""; }
  virtual TSolution Solve(const TProblem&) { return {}; }
};
}  // namespace solvers

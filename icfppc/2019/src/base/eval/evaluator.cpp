#include "base/eval/evaluator.h"

#include "base/decode.h"
#include "base/eval/world.h"

namespace base {
namespace eval {
Evaluator::TResult Evaluator::operator()(const std::string& problem,
                                         const std::string& solution) const {
  if (solution.empty()) return {};
  World world;
  world.Init(problem);
  auto s = DecodeSolution(solution);
  world.Apply(s);
  return {world.Solved(), world.GetTime()};
}
}  // namespace eval
}  // namespace base

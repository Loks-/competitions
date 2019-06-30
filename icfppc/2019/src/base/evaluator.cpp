#include "base/evaluator.h"

#include "base/decode.h"
#include "base/world.h"

namespace base {
Evaluator::TResult Evaluator::operator()(const std::string& problem,
                                         const std::string& solution) const {
  if (solution.empty()) return {};
  World world;
  world.Init(problem);
  auto s = DecodeSolution(solution);
  world.Apply(s);
  return {world.Solved(), world.GetTime()};
}
}  // namespace base

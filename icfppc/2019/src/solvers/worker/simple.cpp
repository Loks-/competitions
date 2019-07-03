#include "solvers/worker/simple.h"

#include "base/action.h"
#include "base/ext/path.h"
#include "base/ext/world.h"
#include <string>

namespace solvers {
namespace worker {
std::string Simple::Name() const { return "simple"; }

base::Action NextMove(base::ext::World& world) {
  const auto& map = world.GetMap();
  const auto& worker = world.GetWorker(0);
  return base::ext::Path::PathToTarget(map, map.Index(worker.Location()),
                                       map.Unwrapped());
}

base::ActionsList Simple::Solve(base::ext::World& world) { return {}; }
}  // namespace worker
}  // namespace solvers

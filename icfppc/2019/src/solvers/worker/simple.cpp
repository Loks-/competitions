#include "solvers/worker/simple.h"

#include "base/action.h"
#include "base/action_type.h"
#include "base/ext/path.h"
#include "base/ext/world.h"

#include "common/data_structures/unsigned_set.h"

#include <cassert>
#include <string>

namespace solvers {
namespace worker {
void Simple::Init(base::ext::World&) {}

base::Action Simple::NextMove(base::ext::World& world) {
  return NextMove(world, world.GetMap().Unwrapped());
}

std::string Simple::Name() const { return "simple"; }

base::Action Simple::NextMove(base::ext::World& world,
                              const ds::UnsignedSet& required) {
  assert(!required.Empty());
  const auto& map = world.GetMap();
  const auto& worker = world.WGet();
  return base::ext::Path::PathToTarget(map, map.Index(worker.Location()),
                                       required);
}

base::ActionsList Simple::Solve(base::ext::World& world) {
  Init(world);
  base::ActionsList al;
  for (; !world.Solved();) {
    world.WGet().PickupItem();
    auto a = NextMove(world);
    assert(a.type != base::ActionType::DO_NOTHING);
    assert(a.type != base::ActionType::END);
    world.WApply(a);
    al.push_back(a);
  }
  return al;
}
}  // namespace worker
}  // namespace solvers

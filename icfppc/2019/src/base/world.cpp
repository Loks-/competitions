#include "base/world.h"

#include "assert_exception.h"
#include "base/action.h"
#include "base/action_type.h"
#include "base/map.h"
#include <cassert>

namespace base {
Boosters& World::GetBoosters() { return boosters; }

Map& World::GetMap() { return map; }

int World::GetTime() const { return time; }

void World::Apply(unsigned worker_index, const Action& action) {
  assert(worker_index < workers.size());
  Worker& w = workers[worker_index];
  if (action.type == ActionType::CLONE) {
    w.PickupItem();
    Assert(map.CheckCodeX(w.Location()),
           "Worker should be on CodeX point for Clone action.");
    Worker wnew(w.Location(), *this);
    wnew.Wrap();
    workers.push_back(wnew);
  } else {
    w(action);
  }
}
}  // namespace base

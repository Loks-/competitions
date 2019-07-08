#include "base/ext/world.h"

#include <cassert>

namespace base {
namespace ext {
World::TWorker& World::WGet() {
  assert(WSize() == 1);
  return GetWorker(0);
}

void World::WApply(const Action& action) {
  assert(WSize() == 1);
  AdjustTime(1);
  Apply(0, action);
}

unsigned World::WSize() const { return workers.size(); }

World::TWorker& World::GetWorker(unsigned windex) {
  assert(windex < workers.size());
  return workers[windex];
}

const World::TWorker& World::GetWorker(unsigned windex) const {
  assert(windex < workers.size());
  return workers[windex];
}
}  // namespace ext
}  // namespace base

#include "base/ext/world.h"

#include <cassert>

namespace base {
namespace ext {
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

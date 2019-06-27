#include "base/world.h"

namespace base {
Boosters& World::GetBoosters() { return boosters; }

Map& World::GetMap() { return map; }

int World::GetTime() const { return time; }
}  // namespace base

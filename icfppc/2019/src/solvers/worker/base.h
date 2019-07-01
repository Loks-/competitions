#pragma once

#include "base/action.h"
#include "base/ext/world.h"

namespace solvers {
namespace worker {
class Base {
 public:
  virtual base::ActionsList Solve(base::ext::World& world) { return {}; }
};
}  // namespace worker
}  // namespace solvers

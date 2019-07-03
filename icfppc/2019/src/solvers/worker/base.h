#pragma once

#include "base/action.h"
#include "base/ext/world.h"
#include <string>

namespace solvers {
namespace worker {
class Base {
 public:
  virtual std::string Name() const;
  virtual base::ActionsList Solve(base::ext::World& world);
};
}  // namespace worker
}  // namespace solvers

#pragma once

#include "base/action.h"
#include "base/ext/world.h"
#include "solvers/worker/base.h"
#include <string>

namespace solvers {
namespace worker {
class Simple : public Base {
 public:
  std::string Name() const override;

  base::Action NextMove(base::ext::World& world);
  base::ActionsList Solve(base::ext::World& world) override;
};
}  // namespace worker
}  // namespace solvers

#pragma once

#include "base/action.h"
#include "base/ext/world.h"
#include "solvers/worker/base.h"
#include "common/unsigned_set.h"
#include <string>

namespace solvers {
namespace worker {
class Simple : public Base {
 protected:
  virtual void Init(base::ext::World& world);
  virtual base::Action NextMove(base::ext::World& world);

 public:
  std::string Name() const override;

  static base::Action NextMove(base::ext::World& world,
                               const UnsignedSet& required);

  base::ActionsList Solve(base::ext::World& world) override;
};
}  // namespace worker
}  // namespace solvers

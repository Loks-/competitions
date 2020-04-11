#pragma once

#include "base/action.h"
#include "base/ext/world.h"
#include "solvers/worker/simple.h"
#include "common/data_structures/disjoint_set.h"
#include "common/unsigned_set.h"
#include <string>

namespace solvers {
namespace worker {
class Local : public Simple {
 protected:
  ds::DisjointSet unwrapped_ds;
  UnsignedSet target;
  unsigned events_index;

 protected:
  void ResetTarget(const base::ext::World& world);
  void Init(base::ext::World& world) override;
  void Update(const base::ext::World& world);

  base::Action NextMove(base::ext::World& world) override;

 public:
  std::string Name() const override;
};
}  // namespace worker
}  // namespace solvers

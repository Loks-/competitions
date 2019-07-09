#pragma once

#include "base/encode.h"
#include "base/ext/world.h"
#include "common/solvers/base.h"
#include "common/solvers/psolver.h"
#include <string>

namespace solvers {
template <class TWorker>
class Single : public Base {
 protected:
  TWorker worker;

 public:
  Single() {}
  Single(TWorker& _worker) { worker = _worker; }
  PSolver Clone() const override { return std::make_shared<Single>(*this); }
  std::string Name() const override { return "single_" + worker.Name(); }

  std::string Solve(const std::string& problem) override {
    base::ext::World world;
    world.Init(problem);
    auto alist = worker.Solve(world);
    return base::Encode(alist);
  }
  // bool SkipCacheRead() const override { return true; }
};
}  // namespace solvers

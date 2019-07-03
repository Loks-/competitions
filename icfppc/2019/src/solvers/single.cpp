#include "solvers/single.h"

#include "base/encode.h"
#include "base/ext/world.h"
#include "solvers/worker/base.h"
#include "common/solvers/psolver.h"
#include <memory>
#include <string>

namespace solvers {
Single::Single(worker::Base& _worker) : worker(_worker) {}
PSolver Single::Clone() const { return std::make_shared<Single>(*this); }
std::string Single::Name() const { return "single_" + worker.Name(); }

std::string Single::Solve(const std::string& problem) {
  base::ext::World world;
  world.Init(problem);
  auto alist = worker.Solve(world);
  return base::Encode(alist);
}
}  // namespace solvers

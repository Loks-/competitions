#include "solvers/worker/simple.h"

#include "base/action.h"
#include "base/ext/world.h"

namespace solvers {
namespace worker {
std::string Simple::Name() const { return "simple"; }

base::ActionsList Simple::Solve(base::ext::World& world) { return {}; }
}  // namespace worker
}  // namespace solvers

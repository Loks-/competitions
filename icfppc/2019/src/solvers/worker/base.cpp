#include "solvers/worker/base.h"

#include "base/action.h"
#include "base/ext/world.h"

#include <string>

namespace solvers {
namespace worker {
std::string Base::Name() const { return "base"; }
base::ActionsList Base::Solve(base::ext::World&) { return {}; }
}  // namespace worker
}  // namespace solvers

#pragma once

#include "solvers/worker/base.h"
#include "common/solvers/base.h"
#include <string>

namespace solvers {
class Single : public Base {
 protected:
  worker::Base& worker;

 public:
  Single(worker::Base& _worker);
  PSolver Clone() const override;
  std::string Name() const override;

  std::string Solve(const std::string& problem) override;
};
}  // namespace solvers

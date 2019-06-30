#pragma once

#include "common/solvers/base.h"
#include <memory>
#include <string>

namespace solvers {
class LSCM : public Base {
  PSolver Clone() const override { return std::make_shared<LSCM>(*this); }
  bool SkipCacheWrite() const override { return true; }
  bool SkipBest() const override { return true; }
  bool UseSubDirectory() const override { return false; }
  std::string Name() const override { return "lscm"; }
};
}  // namespace solvers

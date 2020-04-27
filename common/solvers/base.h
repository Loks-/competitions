#pragma once

#include "common/solvers/psolver.h"

#include <string>

namespace solvers {
class Base {
 public:
  Base() {}
  virtual ~Base() {}

  virtual PSolver Clone() const { return 0; }
  virtual bool SkipCacheRead() const { return false; }
  virtual bool SkipCacheWrite() const { return false; }
  virtual bool SkipBest() const { return false; }
  virtual bool UseSubDirectory() const { return true; }

  virtual std::string Name() const { return ""; }
  virtual std::string Solve(const std::string&) { return ""; }
};
}  // namespace solvers

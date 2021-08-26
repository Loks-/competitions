#pragma once

#include "common/data_structures/unsigned_set.h"

#include "problem.h"
#include "solution.h"
#include "solvers/mctp_base.h"

#include <memory>
#include <string>
#include <vector>

namespace src_solvers {
// Try to find solution with maximum number of vertices.
class MCTPVertices : public MCTPBase {
 public:
  using TBase = MCTPBase;

 protected:
  ds::UnsignedSet covered_vertices;
  bool correct_last_run;

 protected:
  void InitSearch(const Problem& p);
  double DScore() const;
  void Run();

 public:
  MCTPVertices(unsigned max_time_in_seconds) : TBase(max_time_in_seconds) {}

  PSolver Clone() const override {
    return std::make_shared<MCTPVertices>(*this);
  }

  // bool SkipSolutionRead() const override { return true; }

  std::string Name() const override {
    return "mctp_vertices_" + std::to_string(max_time_in_seconds);
  }

  Solution Solve(const Problem& p) override;
};
}  // namespace src_solvers

#pragma once

#include "common/solvers/evaluator.h"

namespace base {
namespace eval {
class Evaluator : public solvers::Evaluator {
 public:
  using TBase = solvers::Evaluator;
  using TResult = TBase::Result;

  TResult operator()(const std::string& problem,
                     const std::string& solution) const;
};
}  // namespace eval
}  // namespace base

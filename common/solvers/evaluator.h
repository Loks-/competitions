#pragma once

#include "common/base.h"

#include <string>

namespace solvers {
class Evaluator {
 public:
  struct Result {
    bool correct;
    int64_t score;
  };

  Result operator()(const std::string&, const std::string&) const {
    return {false, 0};
  }

  bool Compare(const Result& l, const Result& r) const {
    return l.correct ? r.correct ? l.score < r.score : true : false;
  }
};
}  // namespace solvers

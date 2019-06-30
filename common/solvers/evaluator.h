#pragma once

#include "common/base.h"
#include <ostream>
#include <string>

namespace solvers {
class Evaluator {
 public:
  struct Result {
    bool correct;
    int64_t score;
  };

  Result operator()(const std::string& problem,
                    const std::string& solution) const {
    return {false, 0};
  }

  bool Compare(const Result& l, const Result& r) const {
    if (!l.correct) return false;
    if (!r.correct) return true;
    return l.score < r.score;
  }
};
}  // namespace solvers

std::ostream& operator<<(std::ostream& os,
                         const solvers::Evaluator::Result& r) {
  return os << (r.correct ? r.score : 0);
}

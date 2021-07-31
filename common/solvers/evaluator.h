#pragma once

#include "common/base.h"

#include <string>

namespace solvers {
class Evaluator {
 public:
  struct Result {
    bool correct;
    int64_t score;

    Result() : correct(false), score(0) {}
    Result(bool _correct, int64_t _score) : correct(_correct), score(_score) {}
  };

  static bool Compare(const Result& l, const Result& r) {
    return l.correct ? r.correct ? l.score < r.score : true : false;
  }
};
}  // namespace solvers

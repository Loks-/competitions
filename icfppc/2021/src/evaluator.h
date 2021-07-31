#pragma once

#include "common/solvers/evaluator.h"

#include <string>

class Problem;
class Solution;

class Evaluator : public solvers::Evaluator {
 public:
  class Result : public solvers::Evaluator::Result {
   public:
    using TBase = solvers::Evaluator::Result;

   public:
    double raw_points;

   public:
    Result() : raw_points(0) {}
    Result(bool _correct, int64_t _score)
        : TBase(_correct, _score), raw_points(0) {}
    Result(const Problem& p, int64_t _score) : TBase(true, _score) {
      UpdateRawPoints(p);
    }

    double DScore() const;
    void UpdateRawPoints(const Problem& p);
  };

  static Result Apply(const Problem& p, const Solution& s);
};

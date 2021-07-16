#pragma once

#include "common/geometry/d2/point.h"

#include <string>

namespace files {
class JSON;
}  // namespace files

class Bonus {
 public:
  enum BonusType { NONE, GLOBALIST, BREAK_A_LEG, WALLHACK, SUPERFLEX };

 protected:
  BonusType type;
  unsigned problem_get;
  unsigned problem_use;
  I2Point position;

 public:
  Bonus() : type(NONE), problem_get(0), problem_use(0) {}

  BonusType Type() const { return type; }

  void LoadFromProblem(const std::string& problem_id, const files::JSON& json);
  void LoadFromSolution(const std::string& solution_id,
                        const files::JSON& json);
};

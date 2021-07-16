#pragma once

#include "bonus.h"

#include "common/geometry/d2/point.h"
#include "common/solvers/solution.h"

#include <string>
#include <vector>

class Solution : public solvers::Solution {
 protected:
  std::vector<I2Point> points;
  Bonus bonus_used;

  // Not saved to file.
  unsigned bonuses_mask;
  std::vector<Bonus> bonuses;

 public:
  const std::vector<I2Point>& GetPoints() const { return points; }
  const Bonus& GetBonusUsed() const { return bonus_used; }

  bool Load(const std::string& _id, const std::string& filename);
  void Save(const std::string& filename) const;
};

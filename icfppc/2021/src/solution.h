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
  Solution() {}
  Solution(const std::string& _id, const std::vector<I2Point>& _points)
      : points(_points) {
    id = _id;
  }

  bool Empty() const { return points.empty(); }

  const std::vector<I2Point>& GetPoints() const { return points; }
  const Bonus& GetBonusUsed() const { return bonus_used; }

 protected:
  bool LoadI(const std::string& _id, const std::string& filename);
  void SaveI(const std::string& filename) const;

 public:
  static std::string GetFileName(const std::string& _id,
                                 const std::string& solver_name);

  bool Load(const std::string& _id, const std::string& solver_name);
  void Save(const std::string& solver_name);
};

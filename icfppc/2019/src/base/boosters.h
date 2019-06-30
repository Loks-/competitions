#pragma once

#include "base/booster_type.h"
#include <array>

namespace base {
class Boosters {
 protected:
  std::array<unsigned, unsigned(BoosterType::END)> count;

 public:
  Boosters();

  void Clear();
  bool Available(BoosterType type) const;
  unsigned Count(BoosterType type) const;
  void Add(BoosterType type);
  void Remove(BoosterType type);
};
}  // namespace base

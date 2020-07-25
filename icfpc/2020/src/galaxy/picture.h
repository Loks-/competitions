#pragma once

#include "common/base.h"

#include <string>
#include <utility>
#include <vector>

namespace galaxy {
class Picture {
 protected:
  std::vector<std::pair<int64_t, int64_t>> v;

 public:
  void Clear();
  void AddPixel(int64_t x, int64_t y);

  std::string ToString() const;
};
}  // namespace galaxy

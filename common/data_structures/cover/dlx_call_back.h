#pragma once

#include <vector>

namespace ds {
namespace cover {
class IDLXCallBack {
  // If output true, the main algorithm stops.
  // If output false, the main algirthm will search next solution.
  bool operator()(const std::vector<size_t>& rows);
};
}  // namespace cover
}  // namespace ds

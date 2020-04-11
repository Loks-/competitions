#pragma once

#include "base/action.h"
#include "common/data_structures/unsigned_set.h"
#include <functional>

class UnsignedSet;

namespace base {
namespace ext {
class Map;

class Path {
 public:
  using TTarget = std::function<bool(unsigned)>;

  static Action PathToTarget(const Map& m, unsigned from, TTarget target);
  static Action PathToTarget(const Map& m, unsigned from,
                             const ds::UnsignedSet& target);
};
}  // namespace ext
}  // namespace base

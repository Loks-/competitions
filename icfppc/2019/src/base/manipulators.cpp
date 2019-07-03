#include "base/manipulators.h"

#include "base/point.h"
#include "base/rotation_type.h"
#include "common/assert_exception.h"
#include <cassert>

namespace base {
Manipulators::Manipulators() {
  v.push_back(Manipulator({0, 0}));
  v.push_back(Manipulator({1, -1}));
  v.push_back(Manipulator({1, 0}));
  v.push_back(Manipulator({1, 1}));
}

void Manipulators::Add(const Point& p) {
  bool correct = false;
  for (const auto& m : v) {
    if (m == p) {
      correct = false;
      break;
    }
    if (m.Neighbor(p)) correct = true;
  }
  Assert(correct, "Attaching manipulator with inccorect coordinates.");
  v.push_back(Manipulator(p));
}

void Manipulators::Rotate(RotationType type) {
  for (auto& m : v) m.Rotate(type);
}

template <class TMap>
void Manipulators::Wrap(const Point& pworker, TMap& map) const {
  for (auto& m : v) m.Wrap(pworker, map);
}

namespace eval {
class Map;
}  // namespace eval
namespace ext {
class Map;
}  // namespace ext

template void Manipulators::Wrap<eval::Map>(const Point& pworker,
                                            eval::Map& map) const;
template void Manipulators::Wrap<ext::Map>(const Point& pworker,
                                           ext::Map& map) const;
}  // namespace base

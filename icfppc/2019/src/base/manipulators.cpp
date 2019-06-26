#include "base/manipulators.h"

#include "base/point.h"
#include "base/rotation_type.h"

namespace base {
Manipulators::Manipulators() {
  v.push_back(Manipulator({0, 0}));
  v.push_back(Manipulator({1, -1}));
  v.push_back(Manipulator({1, 0}));
  v.push_back(Manipulator({1, 1}));
}

void Manipulators::Rotate(RotationType type) {
  for (auto& m : v) m.Rotate(type);
}

void Manipulators::Wrap(const Point& pworker, Map& map) const {
  for (auto& m : v) m.Wrap(pworker, map);
}

}  // namespace base

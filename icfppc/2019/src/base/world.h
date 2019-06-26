#pragma once

#include "base/map.h"

namespace base {
class World {
 protected:
  Map map;

 public:
  Map& GetMap();
};
}  // namespace base

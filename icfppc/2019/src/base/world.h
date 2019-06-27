#pragma once

#include "base/boosters.h"
#include "base/map.h"

namespace base {
class World {
 protected:
  Boosters boosters;
  Map map;
  int time;

 public:
  Boosters& GetBoosters();
  Map& GetMap();
  int GetTime() const;
};
}  // namespace base

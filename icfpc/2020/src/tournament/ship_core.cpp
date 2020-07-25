#include "tournament/ship_core.h"

#include "tournament/world.h"

namespace tournament {
int ShipCore::Size() const { return fuel + lasers + radiators + engines; }

int ShipCore::Cost() const {
  return fuel * FUEL_COST + lasers * LASERS_COST + radiators * RADIATORS_COST +
         engines * ENGINES_COST;
}
}  // namespace tournament

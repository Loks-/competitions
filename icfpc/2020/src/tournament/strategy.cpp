#include "tournament/strategy.h"

#include "tournament/world.h"

namespace tournament {
ShipCore Strategy::SelectCore(int budget) {
  ShipCore s;
  s.lasers = 0;
  s.radiators = 0;
  s.engines = 0;
  budget -= s.lasers * LASERS_COST + s.radiators * RADIATORS_COST +
            s.engines * ENGINES_COST;
  s.fuel = budget / FUEL_COST;
  return s;
}

Actions Strategy::Run() { return {}; }
}  // namespace tournament

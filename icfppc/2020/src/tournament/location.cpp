#include "tournament/location.h"

#include "tournament/world.h"

#include <cstdlib>

Location::Location() : x(0), y(0) {}
Location::Location(int _x, int _y) : x(_x), y(_y) {}

bool Location::IsValid() const {
  int ax = abs(x), ay = abs(y);
  return (ax < FIELD_SIZE) && (ay < FIELD_SIZE) &&
         ((ax > PLANET_SIZE) || (ay > PLANET_SIZE));
}

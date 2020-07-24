#pragma once

#include "tournament/action.h"
#include "tournament/location_with_speed.h"
#include "tournament/ship_core.h"

class Ship {
 public:
  int role;
  int id;
  LocationWithSpeed location;
  ShipCore matter;
  int temperature;
  int critical_temperature;
  int max_fuel_burn_speed;
  Actions applied_commands;
};

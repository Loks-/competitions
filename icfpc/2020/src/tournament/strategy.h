#pragma once

#include "tournament/action.h"
#include "tournament/ship_core.h"

class Strategy {
 protected:
  int role;

 public:
  ShipCore SelectCore(int budget);
  Actions Run();
};

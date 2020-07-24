#pragma once

#include "tournament/acceleration.h"
#include "tournament/location.h"
#include "tournament/ship_core.h"

#include <string>
#include <vector>

class Action {
 public:
  enum Type { ACCELERATE, DETONATE, SHOOT, SPLIT };

  Type type;
  int ship_id;
  Acceleration acceleration;
  Location target;
  int power;
  ShipCore matter;

  int damage;

  static std::string EncodePair(int x, int y);

  std::string Encode() const;
  void Decode(const std::string& s);
};

using Actions = std::vector<Action>;

std::string Encode(const Actions& actions);

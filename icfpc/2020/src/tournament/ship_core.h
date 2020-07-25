#pragma once

namespace tournament {
class ShipCore {
 public:
  int fuel;
  int lasers;
  int radiators;
  int engines;

 public:
  int Size() const;
  int Cost() const;
};
}  // namespace tournament

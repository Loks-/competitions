#pragma once

class Location {
 public:
  int x, y;

  Location();
  Location(int _x, int _y);

  bool IsValid() const;
};

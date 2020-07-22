#include "common/graph/graph.h"
#include "common/graph/graph/distance_to_sink.h"

#include <iostream>
#include <string>

void FindStableOrbits() {
  std::cout << "Init" << std::endl;
  unsigned n = 255 * 255 * 33 * 33;
  DirectedGraph g(n + 1);
  auto valid_in_g = [](int x, int y, int dx, int dy) {
    return x > -128 && x < 128 && y > -128 && y < 128 && dx > -17 && dx < 17 &&
           dy > -17 && dy < 17;
  };
  auto valid_all = [&](int x, int y, int dx, int dy) {
    return valid_in_g(x, y, dx, dy) &&
           !((x >= -16) && (x <= 16) && (y >= -16) && (y <= 16));
  };
  auto id = [](int x, int y, int dx, int dy) {
    return (x + 127) + 255 * ((y + 127) + 255 * ((dx + 16) + 33 * (dy + 16)));
  };

  std::cout << "Building graph" << std::endl;
  for (int x = -127; x <= 127; ++x) {
    for (int y = -127; y <= 127; ++y) {
      for (int dx = -16; dx <= 16; ++dx) {
        for (int dy = -16; dy <= 16; ++dy) {
          int pid = id(x, y, dx, dy);
          if (!valid_all(x, y, dx, dy)) {
            g.AddEdge(pid, n);
          } else {
            int ax = 0, ay = 0;
            if (abs(x) >= abs(y)) {
              ax = (x > 0) ? -1 : 1;
            }
            if (abs(y) >= abs(x)) {
              ay = (y > 0) ? -1 : 1;
            }
            int vx = dx + ax, vy = dy + ay;
            g.AddEdge(pid, valid_all(x + vx, y + vy, vx, vy)
                               ? id(x + vx, y + vy, vx, vy)
                               : n);
          }
        }
      }
    }
  }

  std::cout << "Distance" << std::endl;
  auto vd = DistanceToSink(g, n);

  int x = 40, y = 70, c = 0;
  for (int dx = -16; dx <= 16; ++dx) {
    std::cout << std::endl << dx << ":\t";
    for (int dy = -16; dy <= 16; ++dy) {
      if (vd[id(x, y, dx, dy)] > 250) ++c;
      std::cout << int(vd[id(x, y, dx, dy)]) << "\t";
    }
  }
  std::cout << std::endl << "Stable orbits: " << c << std::endl;
}

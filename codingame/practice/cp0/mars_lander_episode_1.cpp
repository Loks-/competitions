// https://www.codingame.com/training/easy/mars-lander-episode-1

#include "common/geometry/d2/point_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_mars_lander_episode_1() {
  unsigned N;
  cin >> N;
  auto surface = nvector::Read<I2Point>(N);

  bool enabled = false;
  double g = 3.711, ms = 39.5, yt = 100.0;
  for (unsigned i = 1; i < N; ++i) {
    if (surface[i].y == surface[i - 1].y) yt = surface[i].y;
  }

  // game loop
  while (1) {
    int x, y, hs, vs, f, r, p;
    cin >> x >> y >> hs >> vs >> f >> r >> p;
    if (!enabled) {
      double vs4 = vs - 4 * g + 6;
      if (vs4 < -ms) {
        double t = (vs4 + ms) / (g - 4);
        double h = -(vs4 - ms) / 2 * t;
        double y4 = y - 4 * vs;
        if (h > y4 - yt) enabled = true;
      }
    }
    cout << "0 " << (enabled ? "4" : "0") << endl;
  }
}

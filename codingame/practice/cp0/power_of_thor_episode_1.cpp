// https://www.codingame.com/training/easy/power-of-thor-episode-1

#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point_io.h"
#include "common/numeric/utils/sign.h"
#include "common/stl/base.h"

#include <string>

int main_power_of_thor_episode_1() {
  vector<string> vx{"W", "", "E"}, vy{"N", "", "S"};
  I2Point light, thor;
  cin >> light >> thor;
  auto v = light - thor;

  while (1) {
    int dx = Sign(v.dx);
    int dy = Sign(v.dy);
    v += I2Vector(dx, dy);
    cout << vy[dy + 1] << vx[dx + 1] << endl;
  }
}

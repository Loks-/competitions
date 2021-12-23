#include "common/data_structures/mdvector.h"
#include "common/numeric/utils/sign.h"
#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <sstream>

int main_2105() {
  unsigned M = 1000;
  auto vs = nvector::ReadLines();
  for (auto ab : {0, 1}) {
    ds::MDVector<unsigned> v(0u, {M, M});
    for (auto& s : vs) {
      int x0, x1, y0, y1;
      string ss;
      char c;
      istringstream(s) >> x0 >> c >> y0 >> ss >> x1 >> c >> y1;
      int dx = x1 - x0, dy = y1 - y0;
      if ((dx == 0) || (dy == 0) || (ab && (abs(dx) == abs(dy)))) {
        for (int j = 0; j <= max(abs(dx), abs(dy)); ++j)
          v(x0 + j * Sign(dx), y0 + j * Sign(dy)) += 1;
      }
    }
    cout << v.TransformV([](unsigned x) { return (x > 1) ? 1u : 0u; }).Sum()
         << endl;
  }
  return 0;
}

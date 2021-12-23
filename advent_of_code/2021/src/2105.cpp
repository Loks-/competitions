#include "common/data_structures/mdvector.h"
#include "common/numeric/utils/abs.h"
#include "common/numeric/utils/sign.h"
#include "common/stl/base.h"
#include "common/vector/read_from_string.h"
#include "common/vector/read_lines.h"

// Sparse vector (like unordered map) will be faster than MDVector.
int main_2105() {
  unsigned M = 1000;
  auto vs = nvector::ReadLines();
  for (auto ab : {0, 1}) {
    ds::MDVector<unsigned> v(0u, {M, M});
    for (auto& s : vs) {
      auto vv = nvector::ReadFromString<int>(s, " ,->");
      int x0 = vv[0], y0 = vv[1], x1 = vv[2], y1 = vv[3], dx = x1 - x0,
          dy = y1 - y0;
      if ((dx == 0) || (dy == 0) || (ab && (abs(dx) == abs(dy)))) {
        for (int j = 0, jl = max(abs(dx), abs(dy)); j <= jl; ++j)
          v(x0 + j * Sign(dx), y0 + j * Sign(dy)) += 1;
      }
    }
    cout << v.TransformV([](unsigned x) { return (x > 1) ? 1u : 0u; }).Sum()
         << endl;
  }
  return 0;
}

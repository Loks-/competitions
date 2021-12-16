#include "common/geometry/d2/utils/neighbors.h"
#include "common/stl/full.h"
#include "common/stl/pair_io.h"
#include "common/string/utils/split.h"
#include "common/vector/read.h"
#include "common/vector/read_all.h"
#include "common/vector/read_from_line.h"
#include "common/vector/read_lines.h"
#include "common/vector/transform.h"
#include "common/vector/write.h"

int main_base() {
  auto v = nvector::ReadAll<int64_t>();
  //   auto v = nvector::ReadFromLine<int64_t>(',');
  //   auto vs = nvector::ReadLines();
  int64_t r = 0;
  for (auto u : v) {
    r += u;
  }
  cout << r << endl;
  return 0;
}

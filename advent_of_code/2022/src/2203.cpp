#include "common/stl/base.h"
#include "common/string/utils/to_vector.h"
#include "common/vector/intersection.h"
#include "common/vector/read_lines.h"
#include "common/vector/unique.h"

int main_2203() {
  auto Score = [](char c) -> int {
    if ((c >= 'a') && (c <= 'z'))
      return (c - 'a') + 1;
    else
      return (c - 'A') + 27;
  };

  auto vs = nvector::ReadLines();
  int64_t r1 = 0, r2 = 0;
  for (auto s : vs) {
    auto l = s.size() / 2;
    auto v1 = ToVector(s.substr(0, l));
    auto v2 = ToVector(s.substr(l));
    auto vi = nvector::IntersectionVUnsorted(v1, v2);
    r1 += Score(vi[0]);
  }
  for (unsigned i = 0; i < vs.size(); i += 3) {
    auto v0 = ToVector(vs[i]), v1 = ToVector(vs[i + 1]),
         v2 = ToVector(vs[i + 2]);
    auto vi1 = nvector::IntersectionVUnsorted(v0, v1);
    auto vi2 = nvector::IntersectionVUnsorted(vi1, v2);
    r2 += Score(vi2[0]);
  }
  cout << r1 << endl << r2 << endl;
  return 0;
}

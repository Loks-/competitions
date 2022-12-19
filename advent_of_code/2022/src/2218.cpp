#include "common/geometry/d3/base.h"
#include "common/geometry/d3/stl_hash/point.h"
#include "common/geometry/d3/utils/box.h"
#include "common/geometry/d3/utils/neighbors.h"
#include "common/stl/base.h"
#include "common/vector/read_from_string.h"
#include "common/vector/read_lines.h"

#include <queue>

int main_2218() {
  auto vs = nvector::ReadLines();
  unordered_set<I3Point> points;
  unsigned r1 = 0, r2 = 0;
  for (auto s : vs) {
    auto v = nvector::ReadFromString<int>(s, ",");
    points.insert({v[0], v[1], v[2]});
  }
  for (auto p : points) {
    for (auto p2 : I3NeighborsD6(p)) {
      if (points.find(p2) == points.end()) ++r1;
    }
  }
  I3Vector v1(1, 1, 1);
  auto b = Box(vector<I3Point>{points.begin(), points.end()});
  b.p1 -= v1;
  b.p2 += v1;
  unordered_set<I3Point> points2 = points;
  points2.insert(b.p1);
  queue<I3Point> q;
  for (q.push(b.p1); !q.empty(); q.pop()) {
    for (auto p2 : I3NeighborsD6(q.front())) {
      if (!b.Inside(p2)) continue;
      if (points.find(p2) != points.end()) ++r2;
      if (points2.find(p2) == points2.end()) {
        q.push(p2);
        points2.insert(p2);
      }
    }
  }
  cout << r1 << endl << r2 << endl;
  return 0;
}

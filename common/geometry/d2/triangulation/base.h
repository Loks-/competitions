#pragma once

#include "common/geometry/d2/compare/point_yx.h"
#include "common/geometry/d2/convex_polygon.h"
#include "common/geometry/d2/point.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/triangle_positive_area.h"
#include "common/vector/rotate.h"

#include <algorithm>
#include <vector>

// Based on Graham Scan algorithm for convex hull.
namespace geometry {
namespace d2 {
namespace triangulation {
template <class T>
class Base {
 public:
  using TPoint = Point<T>;
  using TSegment = Segment<T, false>;
  using TTriangle = TrianglePA<T>;
  using THull = ConvexPolygon<T>;
  using TSet = std::vector<TPoint>;
  using TSelf = Base<T>;

 protected:
  std::vector<TSegment> segments;
  std::vector<TTriangle> triangles;
  THull hull;

 public:
  Base() {}
  Base(const TSet& s) { Build(s); }

  Base(const TSelf& t1, const TSelf& t2, const TSegment& s) {
    Build(t1, t2, s);
  }

  void Clear() {
    segments.clear();
    triangles.clear();
  }

  const std::vector<TSegment>& GetSegments() const { return segments; }
  const std::vector<TTriangle>& GetTrianlges() const { return triangles; }

  void Build(const TSet& s) {
    Clear();
    std::vector<Point<T>> v(s), vh(v.size());
    unsigned ilowest = 0;
    for (unsigned i = 1; i < v.size(); ++i) {
      if (CompareYX(v[i], v[ilowest])) ilowest = i;
    }
    if (ilowest > 0) std::swap(v[0], v[ilowest]);
    std::sort(v.begin() + 1, v.end(),
              [&](const Point<T>& p1, const Point<T>& p2) {
                return (p2 - v[0]) % (p1 - v[0]) < 0;
              });

    vh[0] = v[0];
    unsigned i = 1, j = 0;
    for (; i < v.size(); ++i) {
      segments.push_back({v[0], v[i]});
      if (j > 0) {
        segments.push_back({vh[j], v[i]});
        triangles.push_back({v[0], vh[j], v[i]});
      }
      for (; (j > 1) && (((vh[j - 1] - vh[j]) % (v[i] - vh[j])) >= 0);) {
        segments.push_back({vh[j - 1], v[i]});
        triangles.push_back({vh[j - 1], v[i], vh[j]});
        --j;
      }
      vh[++j] = v[i];
    }
    vh.resize(j + 1);
    hull = THull(vh);
  }

  // Merge 2 triangulations to one.
  // Triangulations should be on different sides from common segment
  void Build(const TSelf& t1, const TSelf& t2, const TSegment& common_segment) {
    Clear();
    if (t2.triangles.empty()) {
      *this = t1;
      return;
    }
    if (t1.triangles.empty()) {
      *this = t2;
      return;
    }
    TSegment s1(common_segment);
    unsigned i1 = 0, j1 = 0;
    for (; i1 < t1.hull.Size(); ++i1) {
      if (t1.hull[i1] == s1.p1) break;
    }
    assert(i1 < t1.hull.Size());
    if (t1.hull.MGet(i1 + 1) != s1.p2) {
      auto i0 = (i1 + t1.hull.Size() - 1) % t1.hull.Size();
      assert(t1.hull[i0] == s1.p2);
      std::swap(s1.p1, s1.p2);
      i1 = i0;
    }
    for (; j1 < t2.hull.Size(); ++j1) {
      if (t2.hull[j1] == s1.p2) break;
    }
    assert(j1 < t2.hull.Size());
    assert(t2.hull.MGet(j1 + 1) == s1.p1);
    TSegment s2(s1.p2, s1.p1);

    // Copy segments and triangles
    for (auto& ss : t1.segments) segments.push_back(ss);
    for (auto& ss : t2.segments) {
      if ((ss == s1) || (ss == s2)) continue;
      segments.push_back(ss);
    }
    for (auto& t : t1.triangles) triangles.push_back(t);
    for (auto& t : t2.triangles) triangles.push_back(t);

    // Convex hull
    auto vt1 = t1.hull.v, vt2 = t2.hull.v;
    nvector::RotateLeft(vt1, i1 + 1);
    nvector::RotateLeft(vt2, j1);
    vt1.insert(vt1.end(), vt2.begin() + 2, vt2.end());
    for (auto p : {s1.p2, s1.p1}) {
      if (vt1[0] != p) {
        auto it = std::find(vt1.begin() + 1, vt1.end(), p);
        assert(it != vt1.end());
        nvector::RotateLeft(vt1, it - vt1.begin());
      }
      unsigned k0 = 0, k1 = vt1.size() - 1;
      for (;;) {
        if (((vt1[k0 + 1] - vt1[k0]) % (vt1[k1] - vt1[k0])) < 0) {
          segments.push_back({vt1[k0 + 1], vt1[k1]});
          triangles.push_back({vt1[k0], vt1[k1], vt1[k0 + 1]});
          ++k0;
        } else if (((vt1[k0] - vt1[k1]) % (vt1[k1 - 1] - vt1[k1])) < 0) {
          segments.push_back({vt1[k0], vt1[k1 - 1]});
          triangles.push_back({vt1[k0], vt1[k1], vt1[k1 - 1]});
          --k1;
        } else {
          break;
        }
      }
      vt1.erase(vt1.begin() + k1 + 1, vt1.end());
      vt1.erase(vt1.begin(), vt1.begin() + k0);
    }
    hull = THull(vt1);
  }
};
}  // namespace triangulation
}  // namespace d2
}  // namespace geometry

#include "common/geometry/d2/angle_fraction.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/reflection.h"
#include "common/geometry/d2/reflection/apply_polygon.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/stl_hash/point.h"
#include "common/geometry/d2/triangle.h"
#include "common/geometry/d2/utils/inside_segment_polygon.h"
#include "common/geometry/d2/utils/subtraction_polygon.h"
#include "common/numeric/fraction_io.h"
#include "common/numeric/stl_hash/fraction.h"
#include "common/stl/base.h"

#include <unordered_map>

using namespace geometry::d2;

using TPoint = Point<TIFraction>;
using TPolygon = Polygon<TIFraction>;
using TReflection = Reflection<TIFraction>;
using TSegment = Segment<TIFraction, true>;
using TTriangle = Triangle<TIFraction>;

int main_napkin_folding() {
  unsigned T, N, K;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> N >> K;
    vector<TPoint> vp(N);
    for (unsigned i = 0; i < N; ++i) {
      int64_t x, y;
      cin >> x >> y;
      vp[i] = TPoint(TIFraction(x), TIFraction(y));
    }
    auto plgn0 = TPolygon(vp);

    auto afull = plgn0.DoubleArea(), aone = afull / K;
    vector<TIFraction> vpp(1, TIFraction(0));
    for (unsigned j = 2; j <= K; ++j) {
      for (unsigned i = 1; i < j; ++i) {
        TIFraction f(i, j);
        if (f.GetD() == j) vpp.push_back(f);
      }
    }
    sort(vpp.begin(), vpp.end());
    unsigned l = vpp.size();
    vector<TPoint> vpe;
    for (unsigned i = 0; i < plgn0.Size(); ++i) {
      auto& p = plgn0[i];
      auto v = plgn0.MGet(i + 1) - p;
      for (unsigned j = 0; j < l; ++j) vpe.push_back(p + v * vpp[j]);
    }
    unordered_map<TPoint, unsigned> mpoints;
    for (unsigned i = 0; i < vpe.size(); ++i) mpoints[vpe[i]] = i;

    vector<pair<TPoint, TPoint>> vsol;
    vector<pair<TPolygon, TPolygon>> vtasks;
    for (unsigned i = 0; (i < vpe.size()) && vsol.empty(); ++i) {
      auto acurrent = TIFraction(0);
      auto pi = vpe[i];
      Location lpi{(i % l) ? Location::EDGE : Location::VERTEX, i / l};
      for (unsigned j0 = i / l + 1; j0 < plgn0.Size(); ++j0) {
        auto pj0 = plgn0[j0], pj1 = plgn0.MGet(j0 + 1);
        auto acurrent1 = acurrent + TTriangle(pi, pj0, pj1).SignedDoubleArea();
        unsigned dj = ((acurrent - aone) * (acurrent1 - aone) >= 0) ? l : 1;
        for (unsigned j = j0 * l; j < (j0 + 1) * l; j += dj) {
          auto pj = vpe[j];
          Location lpj{(j % l) ? Location::EDGE : Location::VERTEX, j0};
          auto aj = TTriangle(pi, pj0, pj).SignedDoubleArea();
          if (acurrent + aj == aone) {
            // Candidate from the area point of view
            if (!InisideSegmentPolygon<TIFraction>::InsideI(TSegment(pi, pj),
                                                            plgn0, lpi, lpj))
              continue;
            bool eok = true;
            vector<TPoint> vc{pj, pi}, vl{pi, pj};
            for (unsigned k = l * (i / l + 1); k < j; k += l)
              vc.push_back(vpe[k]);
            for (unsigned k = l * (j / l + 1); k < i + vpe.size(); k += l)
              vl.push_back(vpe[k % vpe.size()]);
            vtasks.clear();
            vtasks.push_back({TPolygon(vl), TPolygon(vc)});
            for (; eok && !vtasks.empty();) {
              TPolygon plgn_l = vtasks.back().first,
                       plgn_s = vtasks.back().second;
              vtasks.pop_back();
              bool found = false;
              for (unsigned k = 0; k < plgn_s.Size(); ++k) {
                auto &pk0 = plgn_s[k], &pk1 = plgn_s.MGet(k + 1);
                auto l0 = Locate(pk0, plgn_l);
                if (l0.type == Location::OUTSIDE) continue;
                auto l1 = Locate(pk1, plgn_l);
                if (l1.type == Location::OUTSIDE) continue;
                if ((l0.type == Location::INSIDE) ||
                    (l1.type == Location::INSIDE)) {
                  eok = false;
                  break;
                }
                if (!Inside(TSegment(pk0, pk1), plgn_l)) continue;
                // Reflection needed
                vsol.push_back({pk1, pk0});
                TReflection r(pk0, pk1);
                auto plgn_sr = Apply(r, plgn_s);
                // Check that all points are on border
                for (auto& p : plgn_sr.Vertices()) {
                  if (mpoints.find(p) == mpoints.end()) {
                    eok = false;
                    break;
                  }
                }
                if (eok && (plgn_sr != plgn_l)) {
                  auto vnew = Subtraction(plgn_l, plgn_sr);
                  if (vnew.empty()) eok = false;
                  for (auto& plgn : vnew) vtasks.push_back({plgn, plgn_sr});
                }
                found = true;
                break;
              }
              if (!found) eok = false;
            }
            if (eok) break;
            vsol.clear();
          }
        }
        acurrent = acurrent1;
      }
    }

    cout << "Case #" << it << ": "
         << (!vsol.empty() ? "POSSIBLE" : "IMPOSSIBLE") << endl;
    if (!vsol.empty()) {
      for (auto& pp : vsol) {
        for (auto p : {pp.first, pp.second}) cout << p.x << " " << p.y << " ";
        cout << endl;
      }
    }
  }
  return 0;
}

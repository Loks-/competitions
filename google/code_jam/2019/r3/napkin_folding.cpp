#include "common/geometry/d2/angle_fraction.h"
#include "common/geometry/d2/base.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/reflection.h"
#include "common/geometry/d2/reflection/apply_polygon.h"
#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/triangle.h"
#include "common/geometry/d2/utils/inside_segment_polygon.h"
#include "common/geometry/d2/utils/subtraction_polygon.h"
#include "common/numeric/fraction_io.h"
#include "common/stl/base.h"

#include "common/vector/write.h"

using namespace geometry::d2;
using namespace geometry::d2::location;

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

    vector<pair<TPoint, TPoint>> vsol;
    vector<TPolygon> vplgn_s, vplgn_l;
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
            if (!hidden::InsideI(TSegment(pi, pj), plgn0, lpi, lpj)) continue;
            bool eok = true;
            vector<TPoint> vc{pj, pi}, vl{pi, pj};
            for (unsigned k = l * (i / l + 1); k < j; k += l)
              vc.push_back(vpe[k]);
            for (unsigned k = l * (j / l + 1); k < i + vpe.size(); k += l)
              vl.push_back(vpe[k % vpe.size()]);
            vplgn_s.clear();
            vplgn_l.clear();
            vplgn_s.push_back(TPolygon(vc));
            vplgn_l.push_back(TPolygon(vl));
            for (unsigned is = 0; eok && (is < vplgn_s.size());) {
              // cout << "\tStep " << vsol.size() << endl;
              // for (unsigned ii = is; ii < vplgn_s.size(); ++ii) {
              //   cout << "\tC " << ii << " = ";
              //   nvector::Write(vplgn_s[ii].v);
              // }
              // for (unsigned ii = 0; ii < vplgn_l.size(); ++ii) {
              //   cout << "\tL " << ii << " = ";
              //   nvector::Write(vplgn_l[ii].v);
              // }
              auto& plgn_s = vplgn_s[is];
              bool found = false;
              for (unsigned il = 0; !found && (il < vplgn_l.size()); ++il) {
                auto& plgn_l = vplgn_l[il];
                for (unsigned k = 0; k < plgn_s.Size(); ++k) {
                  auto &pk0 = plgn_s[k], &pk1 = plgn_s.MGet(k + 1);
                  auto l0 = Locate(pk0, plgn_l);
                  assert(l0.type != Location::INSIDE);
                  if (l0.type == Location::OUTSIDE) continue;
                  auto l1 = Locate(pk1, plgn_l);
                  assert(l1.type != Location::INSIDE);
                  if (l1.type == Location::OUTSIDE) continue;
                  if (!Inside(TSegment(pk0, pk1), plgn_l)) continue;
                  // Reflection needed
                  found = true;
                  vsol.push_back({pk1, pk0});
                  TReflection r(pk0, pk1);
                  auto plgn_sr = Apply(r, plgn_s);
                  if (plgn_sr != plgn_l) {
                    auto vnew = Subtraction(plgn_l, plgn_sr);
                    if (vnew.empty()) eok = false;
                    for (auto& plgn : vnew) vplgn_l.push_back(plgn);
                  }
                  vplgn_s.push_back(plgn_sr);
                  vplgn_l.erase(vplgn_l.begin() + il);
                  break;
                }
              }
              is += found ? 0 : 1;
            }
            if (eok && vplgn_l.empty()) break;
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

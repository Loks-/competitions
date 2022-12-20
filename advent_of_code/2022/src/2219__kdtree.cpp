#include "common/geometry/ds/base.h"
#include "common/geometry/kdtree/ds/base_tree.h"
#include "common/heap.h"
#include "common/linear_algebra/utils/all_values_compare.h"
#include "common/linear_algebra/vector_static_size.h"
#include "common/stl/base.h"
#include "common/vector/extract_int.h"
#include "common/vector/read_lines.h"

int main_2219__kdtree() {
  using TVector = la::VectorStaticSize<int, 4>;
  using TTree = geometry::kdtree::DSBaseTree<9, unsigned, unsigned>;
  using TPoint = TTree::TPoint;
  using TPVector = geometry::ds::Vector<9, unsigned>;

  class State {
   public:
    unsigned t;
    TVector v1, v2;
    int eval;
    int64_t eval2;

    void Eval(const vector<TVector>& vcost) {
      static vector<TVector> vv(40);
      for (unsigned it = 0; it <= t; ++it) vv[it] = v1 * int(it) + v2;
      for (unsigned it = 0, j = 1, c = vcost[0](0); it < t; ++it) {
        if (vv[it](0) >= int(j * c)) {
          ++j;
          for (unsigned it2 = it + c + 1; it2 <= t; ++it2)
            vv[it2](0) += (it2 - it - c);
        }
      }
      for (unsigned k = 1; k < 4; ++k) {
        for (unsigned it = 0, j = 1, c = vcost[k](k - 1); it < t; ++it) {
          if (vv[it](k - 1) >= int(j * c)) {
            ++j;
            for (unsigned it2 = it + 1; it2 <= t; ++it2)
              vv[it2](k) += (it2 - it);
          }
        }
      }
      eval = vv[t](3);
      eval2 = eval;
      for (unsigned i = 4; i--;) eval2 = (eval2 << 8) + int(t) * v1(i) + v2(i);
    }

    bool operator<(const State& r) const { return eval2 < r.eval2; }
    bool operator>(const State& r) const { return eval2 > r.eval2; }

    TPoint Point() const {
      TPoint p;
      p[0] = t;
      for (unsigned i = 0; i < 4; ++i) {
        p[i + 1] = v1(i);
        p[i + 5] = v2(i);
      }
      return p;
    }
  };

  auto Solve = [](string s, unsigned t) {
    vector<TVector> vcost(4);
    auto vt = nvector::ExtractInt<unsigned>(s, " ");
    swap(vt[3], vt[4]);
    for (unsigned i = 0; i < 4; ++i) vcost[i](0) = vt[i];
    vcost[2](1) = vt[4];
    vcost[3](2) = vt[5];
    int m0 = max(max(vcost[1](0), vcost[2](0)), vcost[3](0));
    State s0;
    s0.t = t;
    s0.v1(0) = 1;
    s0.Eval(vcost);
    HeapMaxOnTop<State> h;
    TTree tree;
    tree.Init(TPoint(), TPoint(t * t), 0u);
    TPVector pv1(1u);
    int best = 0;
    for (h.Add(s0); !h.Empty();) {
      auto st = h.Extract();
      if (st.eval <= best) break;
      if (st.t == 0) {
        best = max(best, st.v2(3));
        continue;
      }
      auto pp = st.Point();
      if (tree.Get(pp)) continue;
      tree.Set(TPoint(), pp + pv1, 1);
      for (unsigned i = 0; i <= 4; ++i) {
        auto st2 = st;
        if (i < 4) {
          if (!la::AVLessOrEqual(vcost[i], st2.v2)) continue;
          if ((i == 0) && (st2.v1(0) >= m0)) continue;
          st2.v2 -= vcost[i];
          st2.v1(i) += 1;
          st2.v2(i) -= 1;
        }
        st2.t -= 1;
        st2.v2 += st2.v1;
        st2.Eval(vcost);
        h.Add(st2);
      }
    }
    return best;
  };

  auto vs = nvector::ReadLines();
  unsigned r1 = 0, r2 = 1;
  for (unsigned i = 0; i < vs.size(); ++i) r1 += Solve(vs[i], 24) * (i + 1);
  for (unsigned i = 0; i < 3; ++i) r2 *= Solve(vs[i], 32);
  cout << r1 << endl << r2 << endl;
  return 0;
}

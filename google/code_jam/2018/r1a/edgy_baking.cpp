#include "common/stl/base.h"

#include <cmath>
#include <iomanip>

namespace {
using pud = pair<unsigned, double>;

struct IS {
  vector<pud> v, vt2;
  unsigned P;
  bool found;

  IS(unsigned _P) : P(_P), found(false) {
    v.push_back(make_pair(0, 0.));
    v.push_back(make_pair(P + 1, P + 1.));
  }

  void AddI(const pud& pnew, pud& current) {
    if (current.second < pnew.first) {
      vt2.push_back(current);
      current = pnew;
    } else {
      current.second = max(current.second, pnew.second);
    }
  }

  bool Add(const pud& p) {
    vector<pud> vt;
    vt.reserve(v.size());
    for (const pud& pv : v)
      vt.push_back(make_pair(pv.first + p.first, pv.second + p.second));
    vt2.resize(0);
    pud cur = v[0];
    unsigned i = 1, j = 0;
    for (; (v[i].first <= P) || (vt[j].first <= P);) {
      if (v[i].first <= vt[j].first)
        AddI(v[i++], cur);
      else
        AddI(vt[j++], cur);
    }
    vt2.push_back(cur);
    if (cur.second >= P) found = true;
    vt2.push_back(v.back());
    v.swap(vt2);
    return found;
  }
};
}  // namespace

int main_edgy_baking() {
  cout << setprecision(6) << fixed;
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned N, P, H, W, P0 = 0;
    cin >> N >> P;
    vector<pair<unsigned, double>> v(N);
    unsigned mHW = 500;
    double total_extra = 0.;
    for (unsigned i = 0; i < N; ++i) {
      cin >> W >> H;
      P0 += 2 * (H + W);
      v[i] = make_pair(2 * min(H, W), 2.0 * sqrt(1.0 * (H * H + W * W)));
      mHW = min(mHW, v[i].first);
      total_extra += v[i].second;
    }
    sort(v.begin(), v.end());
    IS iset(P - P0);
    for (unsigned i = 0; i < N; ++i) {
      if (iset.Add(v[i])) break;
    }
    if (iset.found) {
      cout << "Case #" << it << ": " << double(P) << endl;
    } else {
      iset.v.pop_back();
      cout << "Case #" << it << ": " << P0 + iset.v.back().second << endl;
    }
  }
  return 0;
}

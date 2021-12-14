#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/sum.h"

int main_falling_balls() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cout << "Case #" << it << ": ";
    unsigned N;
    cin >> N;
    vector<unsigned> vc = nvector::Read<unsigned>(N);
    unsigned s = nvector::Sum(vc);
    if ((s != N) || (vc[0] == 0) || (vc.back() == 0)) {
      cout << "IMPOSSIBLE" << endl;
      continue;
    }
    vector<unsigned> vp;
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < vc[i]; ++j) vp.push_back(i);
    }
    unsigned md = 0;
    for (unsigned i = 0; i < N; ++i) {
      unsigned d = (vp[i] >= i) ? (vp[i] - i) : (i - vp[i]);
      md = max(md, d);
    }
    cout << md + 1 << endl;
    for (unsigned h = md; h; --h) {
      for (unsigned i = 0; i < N; ++i) {
        cout << ((vp[i] >= i + h) ? "\\" : (vp[i] + h <= i) ? "/" : ".");
      }
      cout << endl;
    }
    for (unsigned i = 0; i < N; ++i) cout << ".";
    cout << endl;
  }
  return 0;
}

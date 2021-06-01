#include "common/heap/ukvm/dheap.h"
#include "common/stl/base.h"

using THeap = heap::ukvm::DHeap<4u, unsigned>;

int main_security_update() {
  unsigned T, C, D;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> C >> D;
    vector<unsigned> vt(C, 0), vb(C, 0), vd(C, 0);
    THeap hb(C), ht(C);
    for (unsigned i = 1; i < C; ++i) {
      int t;
      cin >> t;
      if (t > 0) {
        vt[i] = unsigned(t);
        ht.Set(i, vt[i]);
      } else {
        vb[i] = unsigned(-t);
        hb.Set(i, vb[i]);
      }
    }
    vd[0] = 1;
    for (unsigned done = 1, time = 0; done < C;) {
      if (!hb.Empty() && (hb.TopValue() == done)) {
        unsigned k = 0;
        ++time;
        for (; !hb.Empty() && (hb.TopValue() == done); hb.Pop()) {
          auto v = hb.TopKey();
          vt[v] = time;
          ++k;
        }
        done += k;
      } else {
        assert(ht.Size() > 0);
        time = ht.ExtractValue();
        ++done;
      }
    }
    cout << "Case #" << it << ":";
    for (unsigned i = 0; i < D; ++i) {
      unsigned k1, k2;
      cin >> k1 >> k2;
      unsigned t1 = vt[k1 - 1], t2 = vt[k2 - 1];
      cout << " " << ((t1 < t2) ? (t2 - t1) : max(t1 - t2, 1u));
    }
    cout << endl;
  }
  return 0;
}

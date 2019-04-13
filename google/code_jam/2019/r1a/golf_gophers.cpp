#include "common/modular/utils/merge_remainders.h"
#include "common/stl/base.h"

int main_golf_gophers() {
  vector<unsigned> vq{17, 16, 13, 11, 9, 7};
  unsigned T, N, M, L = 18;
  cin >> T >> N >> M;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned r = 0, qq = 1, rq, t;
    for (unsigned q : vq) {
      for (unsigned i = 0; i < L; ++i) cout << q << " ";
      cout << endl;
      rq = 0;
      for (unsigned i = 0; i < L; ++i) {
        cin >> t;
        rq += t;
      }
      rq %= q;
      r = MergeRemainders<>(qq, r, q, rq);
      qq *= q;
    }
    assert((r <= M) && (qq > M));
    cout << r << endl;
    cin >> t;
    if (t != 1) {
      assert(false);
      break;
    }
  }
  return 0;
}

#include "common/stl/base.h"

#include <string>

int main_parenting_partnering_returns() {
  struct A {
    unsigned b, e, i;
  };

  unsigned T, N;
  string impossible = "IMPOSSIBLE";
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    string s(N, 'x');
    bool possible = true;
    unsigned c = 0, j = 0, b, e;
    vector<A> v;
    for (unsigned i = 0; i < N; ++i) {
      cin >> b >> e;
      v.push_back(A{b, e, i});
    }
    sort(v.begin(), v.end(), [](auto l, auto r) { return l.b < r.b; });
    for (auto a : v) {
      if (c <= a.b) {
        s[a.i] = 'C';
        c = a.e;
      } else if (j <= a.b) {
        s[a.i] = 'J';
        j = a.e;
      } else {
        possible = false;
        break;
      }
    }
    cout << "Case #" << it + 1 << ": " << (possible ? s : impossible) << endl;
  }
  return 0;
}

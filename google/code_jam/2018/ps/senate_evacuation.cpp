#include "common/heap.h"
#include "common/stl/base.h"

int main_senate_evacuation() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned N, t, s = 0;
    cin >> N;
    HeapMaxOnTop<pair<unsigned, char>> pq;
    for (unsigned i = 0; i < N; ++i) {
      cin >> t;
      s += t;
      if (t > 0) pq.Add({t, char(i)});
    }
    cout << "Case #" << it << ": ";
    if (s & 1) {
      auto p = pq.Extract();
      printf("%c ", p.second + 'A');
      p.first -= 1;
      if (p.first > 0) pq.Add(p);
    }
    for (;;) {
      for (unsigned i = 0; i < 2; ++i) {
        auto p = pq.Extract();
        printf("%c", p.second + 'A');
        p.first -= 1;
        if (p.first > 0) pq.Add(p);
      }
      if (pq.Empty()) break;
      cout << " ";
    }
    cout << endl;
  }
  return 0;
}

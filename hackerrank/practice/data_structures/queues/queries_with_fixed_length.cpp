// https://www.hackerrank.com/challenges/queries-with-fixed-length

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <deque>

int main_queries_with_fixed_length() {
  unsigned N, Q, d;
  cin >> N >> Q;
  vector<unsigned> v = ReadVector<unsigned>(N);
  for (unsigned iq = 0; iq < Q; ++iq) {
    cin >> d;
    unsigned best = 1000000;
    deque<pair<unsigned, unsigned>> dq;
    for (unsigned i = 0; i < N; ++i) {
      if (i >= d) best = min(best, dq.front().first);
      for (; !dq.empty() && (dq.back().first <= v[i]);) dq.pop_back();
      dq.push_back({v[i], i});
      if (dq.front().second + d <= i) dq.pop_front();
    }
    best = min(best, dq.front().first);
    cout << best << endl;
  }
  return 0;
}

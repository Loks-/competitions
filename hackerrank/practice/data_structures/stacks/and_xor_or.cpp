// https://www.hackerrank.com/challenges/and-xor-or

#include "common/stl/base.h"

#include <stack>

int main_and_xor_or() {
  unsigned N, a, best = 0;
  cin >> N;
  stack<unsigned> s;
  for (unsigned i = 0; i < N; ++i) {
    cin >> a;
    for (; !s.empty() && (s.top() >= a); s.pop()) best = max(best, s.top() ^ a);
    if (!s.empty()) best = max(best, s.top() ^ a);
    s.push(a);
  }
  cout << best << endl;
  return 0;
}

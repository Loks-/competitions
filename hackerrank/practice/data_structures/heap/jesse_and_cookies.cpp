// https://www.hackerrank.com/challenges/jesse-and-cookies

#include "common/stl/base.h"

#include <functional>
#include <queue>

int main_jesse_and_cookies() {
  unsigned N;
  int count = 0;
  uint64_t K, a1, a2;
  priority_queue<uint64_t, vector<uint64_t>, greater<uint64_t>> q;
  cin >> N >> K;
  for (unsigned i = 0; i < N; ++i) {
    cin >> a1;
    q.push(a1);
  }
  for (; (q.size() > 1) && (q.top() < K);) {
    a1 = q.top();
    q.pop();
    a2 = q.top();
    q.pop();
    q.push(a1 + 2 * a2);
    ++count;
  }
  cout << (((q.size() > 0) && (q.top() >= K)) ? count : -1) << endl;
  return 0;
}

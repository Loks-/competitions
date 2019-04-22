// https://www.hackerrank.com/challenges/jesse-and-cookies

#include "common/heap/heap.h"
#include "common/stl/base.h"

int main_jesse_and_cookies() {
  unsigned N;
  int count = 0;
  uint64_t K, a1, a2;
  HeapMinOnTop<uint64_t> q;
  cin >> N >> K;
  for (unsigned i = 0; i < N; ++i) {
    cin >> a1;
    q.Add(a1);
  }
  for (; (q.Size() > 1) && (q.Top() < K);) {
    a1 = q.GetTop();
    a2 = q.GetTop();
    q.Add(a1 + 2 * a2);
    ++count;
  }
  cout << (((q.Size() > 0) && (q.Top() >= K)) ? count : -1) << endl;
  return 0;
}

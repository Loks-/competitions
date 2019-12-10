// https://www.hackerrank.com/challenges/waiter

#include "common/factorization/primes.h"
#include "common/stl/base.h"

#include <stack>

int main_waiter() {
  unsigned N, Q, x;
  cin >> N >> Q;
  stack<unsigned> sa, sb, st;
  for (unsigned in = 0; in < N; ++in) {
    cin >> x;
    sa.push(x);
  }
  vector<uint64_t> primes = GeneratePrimes(10000);
  for (unsigned iq = 0; iq < Q; ++iq) {
    unsigned p = unsigned(primes[iq]);
    for (; !sa.empty(); sa.pop()) {
      if (sa.top() % p)
        st.push(sa.top());
      else
        sb.push(sa.top());
    }
    for (; !sb.empty(); sb.pop()) cout << sb.top() << endl;
    sa.swap(st);
  }
  for (; !sa.empty(); sa.pop()) cout << sa.top() << endl;
  return 0;
}

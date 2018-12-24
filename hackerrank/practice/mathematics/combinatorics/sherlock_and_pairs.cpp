// https://www.hackerrank.com/challenges/sherlock-and-pairs

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <unordered_map>

int main_sherlock_and_pairs() {
  unsigned T, N;
  cin >> T;
  for (unsigned i = 0; i < T; ++i) {
    cin >> N;
    vector<unsigned> va = ReadVector<unsigned>(N);
    unordered_map<unsigned, uint64_t> m;
    for (unsigned a : va) m[a] += 1;
    uint64_t s = 0;
    for (auto p : m) s += p.second * (p.second - 1);
    cout << s << endl;
  }
  return 0;
}

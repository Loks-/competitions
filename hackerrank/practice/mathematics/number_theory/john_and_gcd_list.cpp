// https://www.hackerrank.com/challenges/john-and-gcd-list

#include "common/numeric/utils/lcm.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/write.h"

int main_john_and_gcd_list() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    vector<unsigned> va = ReadVector<unsigned>(N), vb;
    vb.push_back(va[0]);
    for (unsigned i = 1; i < N; ++i) vb.push_back(LCM(va[i - 1], va[i]));
    vb.push_back(va.back());
    WriteVector(vb);
  }
  return 0;
}

#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/rotate.h"

#include <numeric>

int main_2106a() {
  unsigned m = 80;
  vector<uint64_t> vc(9, 0);
  string s;
  cin >> s;
  for (auto ss : Split(s, ',')) vc[stoi(ss)] += 1;
  for (unsigned im = 0; im < m; ++im) {
    nvector::RotateLeft(vc, 1);
    vc[6] += vc[8];
  }
  cout << accumulate(vc.begin(), vc.end(), 0ull) << endl;
  return 0;
}

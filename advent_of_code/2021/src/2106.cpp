#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/rotate.h"
#include "common/vector/sum.h"

int main_2106() {
  unsigned ma = 80, mb = 256;
  vector<uint64_t> vc(9, 0);
  string s;
  cin >> s;
  for (auto ss : Split(s, ',')) vc[stoi(ss)] += 1;
  for (unsigned im = 1; im <= mb; ++im) {
    nvector::RotateLeft(vc, 1);
    vc[6] += vc[8];
    if ((im == ma) || (im == mb)) cout << nvector::Sum(vc) << endl;
  }
  return 0;
}

#include "common/stl/base.h"
#include "common/string/utils/split.h"

int main_2002b() {
  unsigned n, v = 0;
  cin >> n;
  for (unsigned i = 0; i < n; ++i) {
    string s1, s2, s3;
    cin >> s1 >> s2 >> s3;
    auto ss1 = Split(s1, '-');
    if ((s3[stoi(ss1[0]) - 1] == s2[0]) != (s3[stoi(ss1[1]) - 1] == s2[0])) ++v;
  }
  cout << v << endl;
  return 0;
}

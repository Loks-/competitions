#include "common/stl/base.h"
#include "common/string/utils/split.h"

int main_2002a() {
  unsigned n, v = 0;
  cin >> n;
  for (unsigned i = 0; i < n; ++i) {
    string s1, s2, s3;
    cin >> s1 >> s2 >> s3;
    auto ss1 = Split(s1, '-');
    int c = count(s3.begin(), s3.end(), s2[0]);
    if ((c >= stoi(ss1[0])) && (c <= stoi(ss1[1]))) ++v;
  }
  cout << v << endl;
  return 0;
}

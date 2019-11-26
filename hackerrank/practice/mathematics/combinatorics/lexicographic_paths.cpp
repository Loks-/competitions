// https://www.hackerrank.com/challenges/lexicographic-steps

#include "common/numeric/factorial.h"
#include "common/stl/base.h"

#include <string>

int main_lexicographic_paths() {
  numeric::Factorial f;
  unsigned T, x, y;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    uint64_t k;
    cin >> x >> y >> k;
    string s;
    s.reserve(x + y);
    for (; (x > 0) && (y > 0);) {
      uint64_t l = f.BinomialCoefficient(x + y - 1, x - 1);
      if (k < l) {
        --x;
        s += "H";
      } else {
        --y;
        s += "V";
        k -= l;
      }
    }
    for (; x > 0; --x) s += "H";
    for (; y > 0; --y) s += "V";
    cout << s << endl;
  }
  return 0;
}

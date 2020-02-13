#include "common/stl/base.h"
#include "common/string/numer_to_words.h"

// Time: O(T)
// Memory: O(1)
int main_017() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n;
    cin >> n;
    cout << nstring::NumberToWords::Convert(n) << endl;
  }
  return 0;
}

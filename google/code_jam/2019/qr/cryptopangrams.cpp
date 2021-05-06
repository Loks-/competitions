#include "common/data_structures/coordinate_compression.h"
#include "common/numeric/long/unsigned.h"
#include "common/numeric/long/unsigned/gcd.h"
#include "common/numeric/long/unsigned/hash.h"
#include "common/numeric/long/unsigned_io.h"
#include "common/stl/base.h"

int main_cryptopangrams() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    string s;
    unsigned l;
    cin >> s >> l;
    vector<LongUnsigned> v1(l), v2(l + 1);
    for (unsigned i = 0; i < l; ++i) cin >> v1[i];
    for (unsigned i = 1; i < l; ++i) {
      if (v1[i - 1] != v1[i]) {
        v2[i] = GCD(v1[i - 1], v1[i]);
        for (unsigned j = i - 1; j < i; --j) v2[j] = v1[j] / v2[j + 1];
        for (unsigned j = i; j < l; ++j) v2[j + 1] = v1[j] / v2[j];
        break;
      }
    }
    ds::CoordinateCompression<LongUnsigned> cc(v2);
    string s2(l + 1, ' ');
    for (unsigned i = 0; i <= l; ++i) s2[i] = 'A' + char(cc.GetNew(v2[i]));
    cout << "Case #" << it + 1 << ": " << s2 << endl;
  }
  return 0;
}

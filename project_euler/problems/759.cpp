#include "common/modular_io.h"
#include "common/numeric/bits/bits_count.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(log(N))
// Memory: O(1)
int main_759() {
  uint64_t N = PowU<uint64_t>(10, 16) + 1, n = 0;
  TModularD v00, v01, v02, v10, v11, v12, v20, v21, v22;
  for (uint64_t i = 63; i-- > 0;) {
    n = 2 * n + ((N >> i) & 1);
    v22 = v22 * 8 + v21 * 4 + v20 + v12 * 8 + v11 * 8 + v10 * 2 + v02 * 4 +
          v01 * 4 + v00;
    v21 = v21 * 4 + v20 + v11 * 4 + v10 * 2 + v01 * 2 + v00;
    v20 = v20 * 2 + v10 * 2 + v00;
    v12 = v12 * 8 + v11 * 4 + v10 + v02 * 4 + v01 * 4 + v00;
    v11 = v11 * 4 + v10 + v01 * 2 + v00;
    v10 = v10 * 2 + v00;
    v02 = v02 * 8 + v01 * 4 + v00;
    v01 = v01 * 4 + v00;
    v00 = v00 * 2;
    if (n & 1) {
      TModularD g(numeric::BitsCount(n - 1)), k(n - 1);
      v00 += 1;
      v01 += k;
      v02 += k * k;
      v10 += g;
      v11 += g * k;
      v12 += g * k * k;
      v20 += g * g;
      v21 += g * g * k;
      v22 += g * g * k * k;
    }
  }
  cout << v22 << endl;
  return 0;
}

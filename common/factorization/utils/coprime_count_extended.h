#pragma once

#include "common/base.h"

#include <algorithm>
#include <functional>
#include <vector>

// Return count of numbers coprime to list of primes in the interval [1..n].
// Primes should be sorted.
inline uint64_t CoprimeCountExtended(const std::vector<uint64_t>& vprimes,
                                     uint64_t n) {
  std::function<uint64_t(uint64_t, unsigned)> Count = [&](uint64_t l,
                                                          unsigned k) {
    if ((k >= vprimes.size()) || (l < vprimes[k])) return l;
    uint64_t total = l;
    unsigned i = k,
             iend = std::upper_bound(vprimes.begin() + i, vprimes.end(), l) -
                    vprimes.begin();
    bool b = false;
    for (; i < iend; ++i) {
      auto lp = l / vprimes[i];
      if ((lp < vprimes[i]) && (lp * 10 < iend - i)) {
        b = true;
        break;
      }
      total -= Count(lp, i + 1);
    }
    if (b) {
      auto lp = l / vprimes[i];
      auto it =
          std::upper_bound(vprimes.begin() + i, vprimes.begin() + iend, l);
      for (uint64_t j = 1; j <= lp; ++j) {
        auto it2 = std::upper_bound(vprimes.begin() + i, it, l / (j + 1));
        total -= j * (it - it2);
        it = it2;
      }
    }
    return total;
  };
  return Count(n, 0);
}

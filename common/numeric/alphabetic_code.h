#pragma once

#include "common/base.h"
#include "common/numeric/bits/ulog2.h"

#include <numeric>
#include <vector>

namespace numeric {
template <class TTCode = uint64_t, class TLength = unsigned>
class AlphabeticCode {
 public:
  using TCode = TTCode;
  using TSelf = AlphabeticCode<TCode, TLength>;
  static const unsigned nbits = 8 * sizeof(TCode);

  TCode code;
  TLength length;

 public:
  AlphabeticCode() : code(0), length(0) {}
  AlphabeticCode(TCode _code, TLength _length) : code(_code), length(_length) {}

  TCode Adjust() const { return code << (nbits - length - 1); }
  TSelf operator+(const TSelf& r) const {
    return TSelf((code << r.length) | r.code, length + r.length);
  }
  bool operator<(const TSelf& r) const { return Adjust() < r.Adjust(); }

  // All values in vcounts should be positive.
  // TODO: Add compression
  template <class TValue>
  static std::vector<TSelf> Construct(const std::vector<TValue>& vcounts) {
    if (vcounts.empty()) return {};
    TCode z = 0, n = std::accumulate(vcounts.begin(), vcounts.end(), z),
          m = ((~z) / n) >> 1;
    std::vector<TSelf> output;
    output.reserve(vcounts.size());
    TCode s = 0;
    for (auto& v : vcounts) {
      assert(v > 0);
      TCode va = m * TCode(v);
      TCode snew = s + va;
      auto vbits = numeric::ULog2(va);
      output.push_back({(s + snew) >> vbits, nbits - vbits});
      s = snew;
    }
    return output;
  }
};
}  // namespace numeric

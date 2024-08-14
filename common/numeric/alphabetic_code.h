#pragma once

#include "common/base.h"
#include "common/numeric/bits/ulog2.h"
#include "common/templates/bits.h"

#include <numeric>
#include <vector>

namespace numeric {
template <class TCode = uint64_t, class TLength = unsigned>
class AlphabeticCode {
 public:
  static constexpr unsigned nbits = templates::SizeInBits<TCode>();

  using TSelf = AlphabeticCode<TCode, TLength>;

 public:
  TCode code;
  TLength length;

 public:
  constexpr AlphabeticCode() : code(0), length(0) {}

  constexpr AlphabeticCode(TCode _code, TLength _length)
      : code(_code), length(_length) {}

  constexpr TCode Adjust() const { return code << (nbits - length - 1); }

  constexpr TSelf operator+(const TSelf& r) const {
    return TSelf((code << r.length) | r.code, length + r.length);
  }

  constexpr bool operator<(const TSelf& r) const {
    return Adjust() < r.Adjust();
  }

  // All values in vcounts should be positive.
  // TODO: Add compression
  template <class TValue>
  static constexpr std::vector<TSelf> Construct(
      const std::vector<TValue>& vcounts) {
    if (vcounts.empty()) return {};
    const TCode z = 0, n = std::accumulate(vcounts.begin(), vcounts.end(), z),
                m = ((~z) / n) >> 1;
    std::vector<TSelf> output;
    output.reserve(vcounts.size());
    TCode s = 0;
    for (auto& v : vcounts) {
      assert(v > 0);
      TCode va = m * TCode(v);
      TCode snew = s + va;
      const auto vbits = numeric::ULog2(va);
      output.push_back({(s + snew) >> vbits, nbits - vbits});
      s = snew;
    }
    return output;
  }
};
}  // namespace numeric

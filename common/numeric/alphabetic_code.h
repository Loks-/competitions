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
    TValue n = std::accumulate(vcounts.begin(), vcounts.end(), TValue(0));
    auto nbits = numeric::ULog2(n - 1) + 1;
    std::vector<TSelf> output;
    output.reserve(vcounts.size());
    TValue s = 0;
    for (auto& v : vcounts) {
      assert(v > 0);
      TValue snew = s + v;
      if (v == 1) {
        output.push_back({s, nbits});
      } else {
        auto vbits = numeric::ULog2(v);
        output.push_back({((s + snew) / 2) >> (vbits - 1), nbits - vbits + 1});
      }
      s = snew;
    }
    return output;
  }
};
}  // namespace numeric

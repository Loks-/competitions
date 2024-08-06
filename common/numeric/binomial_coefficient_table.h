#pragma once

#include <vector>

// Time and space to initialize to size n is O(n^2).
namespace numeric {
class BinomialCoefficientTable {
 protected:
  uint64_t max_value;
  std::vector<std::vector<uint64_t>> nCr_table;

 public:
  constexpr explicit BinomialCoefficientTable(uint64_t _max_value = (1ull
                                                                     << 62))
      : max_value(_max_value) {}

  constexpr uint64_t MaxValue() const { return max_value; }

  constexpr void Adjust(size_t n) {
    for (; nCr_table.size() <= n;) {
      std::vector<uint64_t> vnext(nCr_table.size() + 1, 1);
      for (size_t i = 1; i + 1 < vnext.size(); ++i)
        vnext[i] =
            std::min(nCr_table.back()[i - 1] + nCr_table.back()[i], max_value);
      nCr_table.push_back(vnext);
    }
  }

  constexpr uint64_t operator()(size_t n, size_t k) {
    if (k > n) return 0;
    Adjust(n);
    return nCr_table[n][k];
  }
};
}  // namespace numeric

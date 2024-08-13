#pragma once

#include "common/base.h"
#include "common/numeric/long/unsigned.h"
#include "common/numeric/long/unsigned/multiplication.h"
#include "common/numeric/long/unsigned/pow.h"

#include <algorithm>
#include <vector>

namespace numeric {
namespace nlong {
class ConvolutionHelper {
 protected:
  static constexpr unsigned MaxSize(const std::vector<Unsigned>& a) {
    unsigned max_size = 0;
    for (auto& ai : a) max_size = std::max<unsigned>(max_size, ai.Size());
    return max_size;
  }

  static constexpr Unsigned V2U(const std::vector<Unsigned>& a,
                                unsigned xsize) {
    Unsigned::TData rd(a.size() * xsize);
    for (unsigned i = 0; i < a.size(); ++i) {
      const auto& ai = a[i].Data();
      for (unsigned j = 0; j < ai.size(); ++j) rd[i * xsize + j] = ai[j];
    }
    return Unsigned(rd);
  }

  static constexpr std::vector<Unsigned> U2V(const Unsigned& a,
                                             unsigned xsize) {
    if (!a.Size()) return {};
    const auto& ad = a.Data();
    const unsigned l = (ad.size() - 1) / xsize + 1;
    std::vector<Unsigned> v(l);
    for (unsigned i = 0; i < l; ++i) {
      v[i] = Unsigned(Unsigned::TData(
          ad.begin() + i * xsize,
          ad.begin() + std::min<unsigned>((i + 1) * xsize, ad.size())));
    }
    return v;
  }

 public:
  static std::vector<Unsigned> Convolution(const std::vector<Unsigned>& a) {
    const unsigned max_size_a = MaxSize(a);
    if (max_size_a == 0) return {};
    const unsigned x_size = 2 * max_size_a + 1;
    const auto vr = U2V(Sqr(V2U(a, x_size)), x_size);
    assert(vr.size() == 2 * a.size() - 1);
    return vr;
  }

  static std::vector<Unsigned> Convolution(const std::vector<Unsigned>& a,
                                           const std::vector<Unsigned>& b) {
    const unsigned max_size_a = MaxSize(a), max_size_b = MaxSize(b);
    if ((max_size_a == 0) || (max_size_b == 0)) return {};
    const unsigned x_size = max_size_a + max_size_b + 1;
    const auto vr = U2V(Mult(V2U(a, x_size), V2U(b, x_size)), x_size);
    assert(vr.size() == a.size() + b.size() - 1);
    return vr;
  }

  static std::vector<Unsigned> ConvolutionN(const std::vector<Unsigned>& a,
                                            unsigned n) {
    const unsigned max_size_a = MaxSize(a);
    if (max_size_a == 0) return {};
    const unsigned x_size = n * max_size_a + n / 16 + 1;
    const auto vr = U2V(PowU(V2U(a, x_size), n), x_size);
    assert(vr.size() == (a.size() - 1) * n + 1);
    return vr;
  }
};

inline std::vector<Unsigned> Convolution(const std::vector<Unsigned>& a) {
  return ConvolutionHelper::Convolution(a);
}

inline std::vector<Unsigned> Convolution(const std::vector<Unsigned>& a,
                                         const std::vector<Unsigned>& b) {
  return ConvolutionHelper::Convolution(a, b);
}

inline std::vector<Unsigned> ConvolutionN(const std::vector<Unsigned>& a,
                                          unsigned n) {
  return ConvolutionHelper::ConvolutionN(a, n);
}
}  // namespace nlong
}  // namespace numeric

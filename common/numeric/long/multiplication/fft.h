#pragma once

#include "common/modular.h"
#include "common/modular/static/fft.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/numeric/long/unsigned.h"

#include <vector>

namespace numeric {
namespace nlong {
namespace multiplication {
// Max supported length for product is 2^25.
class FFT {
 public:
  using TModular1 = ModularPrime32<2013265921>;  // 2^27*3*5 + 1
  using TModular2 = ModularPrime32<1811939329>;  // 2^26*3^2 + 1

 protected:
  static constexpr uint32_t mask16 = (1u << 16) - 1;
  static constexpr unsigned log2_maxn = 25;

  using TMFFT1 = modular::mstatic::FFT<TModular1, log2_maxn + 1u, 31>;
  using TMFFT2 = modular::mstatic::FFT<TModular2, log2_maxn + 1u, 13>;

 protected:
  template <class TModular>
  static constexpr std::vector<TModular> Convert(const Unsigned& a) {
    std::vector<TModular> v;
    v.reserve(2 * a.Size());
    for (auto ai : a) {
      v.push_back(ai & mask16);
      v.push_back(ai >> 16);
    }
    return v;
  }

  static constexpr Unsigned Restore(const std::vector<TModular1>& v1,
                                    const std::vector<TModular2>& v2) {
    assert(v1.size() == v2.size());
    uint64_t t64 = 0;
    Unsigned::TData v(v1.size() / 2, 0);
    for (size_t i = 0; i < v1.size(); ++i) {
      t64 += MergeRemainders<modular::TArithmetic_P32U>(
          TModular1::GetMod(), v1[i].Get(), TModular2::GetMod(), v2[i].Get());
      const uint32_t t16 = uint32_t(t64) & mask16;
      v[i / 2] += (i % 2) ? (t16 << 16) : t16;
      t64 >>= 16;
    }
    for (; t64; t64 >>= 32) v.push_back(uint32_t(t64));
    return Unsigned(v);
  }

 public:
  static consteval uint64_t MaxN() { return (1ull << log2_maxn); }

  static Unsigned Sqr(const Unsigned& a) {
    assert(2 * a.Size() <= MaxN());
    return Restore(TMFFT1::SConvolution(Convert<TModular1>(a)),
                   TMFFT2::SConvolution(Convert<TModular2>(a)));
  }

  static Unsigned Mult(const Unsigned& a, const Unsigned& b) {
    assert((a.Size() + b.Size()) <= (1u << log2_maxn));
    return Restore(
        TMFFT1::SConvolution(Convert<TModular1>(a), Convert<TModular1>(b)),
        TMFFT2::SConvolution(Convert<TModular2>(a), Convert<TModular2>(b)));
  }
};

inline Unsigned SqrFFT(const Unsigned& a) { return FFT::Sqr(a); }

inline Unsigned MultFFT(const Unsigned& a, const Unsigned& b) {
  return FFT::Mult(a, b);
}
}  // namespace multiplication
}  // namespace nlong
}  // namespace numeric

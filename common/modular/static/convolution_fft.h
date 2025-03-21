#pragma once

#include "common/base.h"
#include "common/modular.h"
#include "common/modular/static/fft.h"
#include "common/modular/utils/merge_remainders.h"

#include <vector>

namespace modular {
namespace mstatic {
template <class TModular>
class ConvolutionFFT {
 protected:
  static_assert(TModular::IsModPrime() && TModular::IsMod32Bits());

  static constexpr unsigned log2_maxn = 26;

  using TModularA = modular::TArithmetic_P32U;
  using TModular1 = ModularPrime32<2013265921>;  // 2^27*3*5 + 1
  using TModular2 = ModularPrime32<1811939329>;  // 2^26*3^2 + 1
  using TModular3 = ModularPrime32<469762049>;   // 2^26*7 + 1, 3
  using TMFFT1 = modular::mstatic::FFT<TModular1, log2_maxn, 31>;
  using TMFFT2 = modular::mstatic::FFT<TModular2, log2_maxn, 13>;
  using TMFFT3 = modular::mstatic::FFT<TModular3, log2_maxn, 3>;

 protected:
  template <class TModularNew>
  static constexpr std::vector<TModularNew> ChangeModular(
      const std::vector<TModular>& a) {
    std::vector<TModularNew> v(a.size());
    for (size_t i = 0; i < a.size(); ++i) v[i] = TModularNew(a[i].Get());
    return v;
  }

  static constexpr std::vector<TModular> Restore(
      const std::vector<TModular1>& v1, const std::vector<TModular2>& v2,
      const std::vector<TModular3>& v3) {
    constexpr uint64_t p1 = TModular1::GetMod(), p2 = TModular2::GetMod(),
                       p3 = TModular3::GetMod(), p12 = p1 * p2,
                       p12m3 = p12 % p3;
    assert((v1.size() == v2.size()) && (v1.size() == v3.size()));
    const TModular mp12(p12);
    std::vector<TModular> v(v1.size());
    for (size_t i = 0; i < v1.size(); ++i) {
      const auto v12 =
          MergeRemainders<TModularA>(p1, v1[i].Get(), p2, v2[i].Get());
      const uint64_t p12m =
          TModularA::Div(TModularA::SubSafe(v3[i].Get(), v12, p3), p12m3, p3);
      v[i] = mp12 * TModular(p12m) + TModular(v12);
    }
    return v;
  }

 public:
  static std::vector<TModular> Convolution(const std::vector<TModular>& a) {
    return Restore(TMFFT1::SConvolution(ChangeModular<TModular1>(a)),
                   TMFFT2::SConvolution(ChangeModular<TModular2>(a)),
                   TMFFT3::SConvolution(ChangeModular<TModular3>(a)));
  }

  static std::vector<TModular> Convolution(const std::vector<TModular>& a,
                                           const std::vector<TModular>& b) {
    return Restore(TMFFT1::SConvolution(ChangeModular<TModular1>(a),
                                        ChangeModular<TModular1>(b)),
                   TMFFT2::SConvolution(ChangeModular<TModular2>(a),
                                        ChangeModular<TModular2>(b)),
                   TMFFT3::SConvolution(ChangeModular<TModular3>(a),
                                        ChangeModular<TModular3>(b)));
  }
};
}  // namespace mstatic
}  // namespace modular

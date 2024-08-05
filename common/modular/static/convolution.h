#pragma once

#include "common/base.h"
#include "common/modular.h"
#include "common/modular/static/fft.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/numeric/convolution_base.h"

#include <vector>

namespace modular {
namespace mstatic {
namespace hidden {
template <class TModularNew, class TModular>
constexpr std::vector<TModularNew> ConvolutionFFTChangeModular(
    const std::vector<TModular>& a) {
  std::vector<TModularNew> v(a.size());
  for (size_t i = 0; i < a.size(); ++i) v[i] = TModularNew(a[i].Get());
  return v;
}
}  // namespace hidden

// Max supported length for a is 2^24.
template <class TModular, unsigned maxn = (1u << 16)>
inline std::vector<TModular> ConvolutionFFT(const std::vector<TModular>& a) {
  static_assert(TModular::IsModPrime() && TModular::IsMod32Bits());
  using TModularA = modular::TArithmetic_P32U;
  using TModular1 = TModular_P32<2013265921>;
  using TModular2 = TModular_P32<1811939329>;
  using TModular3 = TModular_P32<1711276033>;
  using TFFT1 = modular::mstatic::FFT<TModular1>;
  using TFFT2 = modular::mstatic::FFT<TModular2>;
  using TFFT3 = modular::mstatic::FFT<TModular3>;
  thread_local TFFT1 fft1(maxn);
  thread_local TFFT2 fft2(maxn);
  thread_local TFFT3 fft3(maxn);
  uint64_t p1 = TModular1::GetMod(), p2 = TModular2::GetMod(),
           p3 = TModular3::GetMod(), p12 = p1 * p2, p12m3 = p12 % p3;
  assert(2 * a.size() <= maxn);
  auto r1 = fft1.Convolution(hidden::ConvolutionFFTChangeModular<TModular1>(a));
  auto r2 = fft2.Convolution(hidden::ConvolutionFFTChangeModular<TModular2>(a));
  auto r3 = fft3.Convolution(hidden::ConvolutionFFTChangeModular<TModular3>(a));
  assert((r1.size() == r2.size()) && (r1.size() == r3.size()));
  TModular mp12(p12);
  std::vector<TModular> v(r1.size());
  for (size_t i = 0; i < r1.size(); ++i) {
    auto r12 = MergeRemainders<TModularA>(p1, r1[i].Get(), p2, r2[i].Get());
    uint64_t p12m =
        TModularA::Div(TModularA::SubSafe(r3[i].Get(), r12, p3), p12m3, p3);
    v[i] = mp12 * TModular(p12m) + TModular(r12);
  }
  return v;
}

// Max supported length for (a + b) is 2^25.
template <class TModular, unsigned maxn = (1u << 16)>
inline std::vector<TModular> ConvolutionFFT(const std::vector<TModular>& a,
                                            const std::vector<TModular>& b) {
  static_assert(TModular::IsModPrime() && TModular::IsMod32Bits());
  using TModularA = modular::TArithmetic_P32U;
  using TModular1 = TModular_P32<2013265921>;
  using TModular2 = TModular_P32<1811939329>;
  using TModular3 = TModular_P32<1711276033>;
  using TFFT1 = modular::mstatic::FFT<TModular1>;
  using TFFT2 = modular::mstatic::FFT<TModular2>;
  using TFFT3 = modular::mstatic::FFT<TModular3>;
  thread_local TFFT1 fft1(maxn);
  thread_local TFFT2 fft2(maxn);
  thread_local TFFT3 fft3(maxn);
  uint64_t p1 = TModular1::GetMod(), p2 = TModular2::GetMod(),
           p3 = TModular3::GetMod(), p12 = p1 * p2, p12m3 = p12 % p3;
  assert(a.size() + b.size() <= maxn);
  auto r1 = fft1.Convolution(hidden::ConvolutionFFTChangeModular<TModular1>(a),
                             hidden::ConvolutionFFTChangeModular<TModular1>(b));
  auto r2 = fft2.Convolution(hidden::ConvolutionFFTChangeModular<TModular2>(a),
                             hidden::ConvolutionFFTChangeModular<TModular2>(b));
  auto r3 = fft3.Convolution(hidden::ConvolutionFFTChangeModular<TModular3>(a),
                             hidden::ConvolutionFFTChangeModular<TModular3>(b));
  assert((r1.size() == r2.size()) && (r1.size() == r3.size()));
  TModular mp12(p12);
  std::vector<TModular> v(r1.size());
  for (size_t i = 0; i < r1.size(); ++i) {
    auto r12 = MergeRemainders<TModularA>(p1, r1[i].Get(), p2, r2[i].Get());
    uint64_t p12m =
        TModularA::Div(TModularA::SubSafe(r3[i].Get(), r12, p3), p12m3, p3);
    v[i] = mp12 * TModular(p12m) + TModular(r12);
  }
  return v;
}

template <class TModular, unsigned maxn = (1u << 16)>
inline std::vector<TModular> Convolution(const std::vector<TModular>& a) {
  return (a.size() < 100) ? numeric::ConvolutionBase(a)
                          : ConvolutionFFT<TModular, maxn>(a);
}

template <class TModular, unsigned maxn = (1u << 16)>
inline std::vector<TModular> Convolution(const std::vector<TModular>& a,
                                         const std::vector<TModular>& b) {
  return ((a.size() < 100) || (b.size() < 100))
             ? numeric::ConvolutionBase(a, b)
             : ConvolutionFFT<TModular, maxn>(a, b);
}
}  // namespace mstatic
}  // namespace modular

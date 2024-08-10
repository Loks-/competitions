#pragma once

#include "common/modular.h"
#include "common/modular/static/fft.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/numeric/long/unsigned.h"

namespace numeric {
namespace nlong {
namespace hidden {
template <class TModular>
constexpr std::vector<TModular> MultFFTConvert(const Unsigned& a) {
  constexpr uint32_t mask16 = (1u << 16) - 1;
  std::vector<TModular> v;
  v.reserve(2 * a.Size());
  for (auto ai : a) {
    v.push_back(ai & mask16);
    v.push_back(ai >> 16);
  }
  return v;
}
}  // namespace hidden

// Max supported length for a is 2^24.
template <unsigned log2_maxn = 16>
inline Unsigned SqrFFT(const Unsigned& a) {
  static_assert(log2_maxn <= 26);
  constexpr uint32_t mask16 = (1u << 16) - 1;
  using TModular1 = TModular_P32<2013265921>;  // 2^27*3*5 + 1
  using TModular2 = TModular_P32<1811939329>;  // 2^26*3^2 + 1
  thread_local modular::mstatic::FFT<TModular1, log2_maxn, 31> fft1;
  thread_local modular::mstatic::FFT<TModular2, log2_maxn, 13> fft2;
  assert(4 * a.Size() <= (1u << log2_maxn));
  const auto r1 = fft1.Convolution(hidden::MultFFTConvert<TModular1>(a));
  const auto r2 = fft2.Convolution(hidden::MultFFTConvert<TModular2>(a));
  assert(r1.size() == r2.size());
  uint64_t t64 = 0;
  Unsigned::TData v(r1.size() / 2, 0);
  for (size_t i = 0; i < r1.size(); ++i) {
    t64 += MergeRemainders<modular::TArithmetic_P32U>(
        TModular1::GetMod(), r1[i].Get(), TModular2::GetMod(), r2[i].Get());
    const uint32_t t16 = uint32_t(t64) & mask16;
    v[i / 2] += (i % 2) ? (t16 << 16) : t16;
    t64 >>= 16;
  }
  for (; t64; t64 >>= 32) v.push_back(uint32_t(t64));
  return Unsigned(v);
}

// Max supported length for (a*b) is 2^25.
template <unsigned log2_maxn = 16>
inline Unsigned MultFFT(const Unsigned& a, const Unsigned& b) {
  static_assert(log2_maxn <= 26);
  constexpr uint32_t mask16 = (1u << 16) - 1;
  using TModular1 = TModular_P32<2013265921>;  // 2^27*3*5 + 1
  using TModular2 = TModular_P32<1811939329>;  // 2^26*3^2 + 1
  thread_local modular::mstatic::FFT<TModular1, log2_maxn, 31> fft1;
  thread_local modular::mstatic::FFT<TModular2, log2_maxn, 13> fft2;
  assert(2 * (a.Size() + b.Size()) <= (1u << log2_maxn));
  const auto r1 = fft1.Convolution(hidden::MultFFTConvert<TModular1>(a),
                                   hidden::MultFFTConvert<TModular1>(b));
  const auto r2 = fft2.Convolution(hidden::MultFFTConvert<TModular2>(a),
                                   hidden::MultFFTConvert<TModular2>(b));
  assert(r1.size() == r2.size());
  uint64_t t64 = 0;
  Unsigned::TData v(r1.size() / 2, 0);
  for (size_t i = 0; i < r1.size(); ++i) {
    t64 += MergeRemainders<modular::TArithmetic_P32U>(
        TModular1::GetMod(), r1[i].Get(), TModular2::GetMod(), r2[i].Get());
    const uint32_t t16 = uint32_t(t64) & mask16;
    v[i / 2] += (i % 2) ? (t16 << 16) : t16;
    t64 >>= 16;
  }
  for (; t64; t64 >>= 32) v.push_back(uint32_t(t64));
  return Unsigned(v);
}
}  // namespace nlong
}  // namespace numeric

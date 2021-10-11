#pragma once

#include "common/modular.h"
#include "common/modular/static/fft.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/numeric/long/unsigned.h"

#include <algorithm>
#include <vector>

namespace numeric {
namespace nlong {
inline Unsigned MultBase(const Unsigned& a, const Unsigned& b) {
  Unsigned r;
  for (auto it = b.end(); it != b.begin();) {
    r.ShiftBlocksRight(1);
    r += a * *(--it);
  }
  return r;
}

namespace hidden {
template <class TModular>
inline std::vector<TModular> MultFFTConvert(const Unsigned& a) {
  static const uint32_t mask16 = (1u << 16) - 1;
  std::vector<TModular> v;
  v.reserve(2 * a.Size());
  for (auto ai : a) {
    v.push_back(ai & mask16);
    v.push_back(ai >> 16);
  }
  return v;
}
}  // namespace hidden

// Max supported length for (a*b) is 2^25.
template <unsigned maxn = (1u << 16)>
inline Unsigned MultFFT(const Unsigned& a, const Unsigned& b) {
  static const uint32_t mask16 = (1u << 16) - 1;
  using TModular1 = TModular_P32<2013265921>;
  using TModular2 = TModular_P32<1811939329>;
  using TFFT1 = modular::mstatic::FFT<TModular1>;
  using TFFT2 = modular::mstatic::FFT<TModular2>;
  thread_local TFFT1 fft1(maxn);
  thread_local TFFT2 fft2(maxn);
  assert(2 * (a.Size() + b.Size()) <= (1u << 26));
  auto r1 = fft1.Convolution(hidden::MultFFTConvert<TModular1>(a),
                             hidden::MultFFTConvert<TModular1>(b));
  auto r2 = fft2.Convolution(hidden::MultFFTConvert<TModular2>(a),
                             hidden::MultFFTConvert<TModular2>(b));
  assert(r1.size() == r2.size());
  uint64_t t64 = 0;
  Unsigned::TData v(r1.size() / 2, 0);
  for (size_t i = 0; i < r1.size(); ++i) {
    t64 += MergeRemainders<modular::TArithmetic_P32U>(
        TModular1::GetMod(), r1[i].Get(), TModular2::GetMod(), r2[i].Get());
    uint32_t t16 = uint32_t(t64) & mask16;
    v[i / 2] += (i % 2) ? (t16 << 16) : t16;
    t64 >>= 16;
  }
  for (; t64; t64 >>= 32) v.push_back(uint32_t(t64));
  return Unsigned(v);
}

inline Unsigned MultAuto(const Unsigned& a, const Unsigned& b) {
  return (std::min(a.Size(), b.Size()) < 100) ? MultBase(a, b) : MultFFT(a, b);
}
}  // namespace nlong
}  // namespace numeric

inline LongUnsigned operator*(const LongUnsigned& l, const LongUnsigned& r) {
  return numeric::nlong::MultAuto(l, r);
}

inline LongUnsigned& operator*=(LongUnsigned& l, const LongUnsigned& r) {
  l = numeric::nlong::MultAuto(l, r);
  return l;
}

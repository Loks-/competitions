#pragma once

#include "common/base.h"
#include "common/factorization/utils/factorization_base.h"
#include "common/modular/utils/primitive_root.h"
#include "common/numeric/bits/ulog2.h"

#include <algorithm>
#include <mutex>
#include <vector>

namespace modular {
namespace mstatic {
template <class TModular, unsigned log2_maxn, unsigned primitive_root>
class FFT {
 public:
  using TVector = std::vector<TModular>;
  using TSelf = FFT<TModular, log2_maxn, primitive_root>;

 protected:
  static constexpr uint64_t p = TModular::GetMod();
  static constexpr unsigned maxn = (1u << log2_maxn);
  static constexpr TModular primitive = primitive_root;

  static_assert(((p - 1) % maxn) == 0);
  static_assert(IsPrimitiveRoot(p, FactorizeBase(p - 1), primitive_root));

 protected:
  mutable std::mutex m;
  mutable std::vector<TVector> roots;
  mutable std::vector<std::vector<unsigned>> bit_rev_map;

 protected:
  constexpr void InitK(unsigned k) const {
    const uint64_t n = (1u << k);
    const TModular nroot = primitive.PowU((p - 1) / n);
    TModular r(1);
    roots.push_back({});
    auto& vr = roots.back();
    vr.resize(n);
    for (unsigned i = 0; i < n; ++i) {
      vr[i] = r;
      r *= nroot;
    }
    assert(r == 1);
    bit_rev_map.push_back({});
    const auto& vbp = bit_rev_map[bit_rev_map.size() - 2];
    auto& vb = bit_rev_map.back();
    vb.resize(n);
    for (unsigned i = 0; i < n; ++i)
      vb[i] = vbp[i / 2] + ((i & 1) ? n / 2 : 0u);
  }

  constexpr void Init() {
    roots.clear();
    roots.reserve(log2_maxn + 1);
    roots.push_back({TModular(1u)});
    bit_rev_map.clear();
    bit_rev_map.reserve(log2_maxn + 1);
    bit_rev_map.push_back({0});
  }

  static constexpr void FFTI_Adjust(TVector& output) {
    std::reverse(output.begin() + 1, output.end());
    const TModular invn = TModular(output.size()).Inverse();
    for (auto& o : output) o *= invn;
  }

 public:
  static constexpr unsigned GetNForFFT(unsigned l) {
    unsigned n = 1;
    for (; n < l;) n *= 2;
    return n;
  }

  static constexpr unsigned MaxN() { return maxn; }

  constexpr FFT() { Init(); }

  void AdjustK(unsigned k) const {
    if (roots.size() <= k) {
      const std::lock_guard<std::mutex> lock(m);
      for (unsigned l = roots.size(); l <= k; ++l) InitK(l);
    }
  }

  constexpr TVector Apply(unsigned n, const TVector& vx) const {
    assert((n > 0) && (maxn % n == 0));
    const unsigned k = numeric::ULog2(n);
    AdjustK(k);
    TVector output(n);
    for (unsigned i = 0; i < n; ++i)
      output[i] = (bit_rev_map[k][i] < vx.size()) ? vx[bit_rev_map[k][i]] : 0;

    TVector current(n);
    for (unsigned l = 1, kl = 1; l < n; l <<= 1, ++kl) {
      const TVector& current_roots = roots[kl];
      for (unsigned pdest = 0; pdest < n; pdest += l) {
        for (unsigned i = 0; i < l; ++i, ++pdest) {
          const TModular v = current_roots[i] * output[pdest + l];
          current[pdest] = output[pdest] + v;
          current[pdest + l] = output[pdest] - v;
        }
      }
      output.swap(current);
    }
    return output;
  }

  constexpr TVector ApplyInv(unsigned n, const TVector& vx) const {
    TVector output = Apply(n, vx);
    FFTI_Adjust(output);
    return output;
  }

  constexpr TVector Convolution(const TVector& vx) const {
    const unsigned n = GetNForFFT(unsigned(2 * vx.size()));
    auto vf = Apply(n, vx);
    for (auto& f : vf) f *= f;
    return ApplyInv(n, vf);
  }

  constexpr TVector Convolution(const TVector& vx1, const TVector& vx2) const {
    const unsigned n = GetNForFFT(unsigned(vx1.size() + vx2.size()));
    auto vf1 = Apply(n, vx1), vf2 = Apply(n, vx2);
    for (unsigned i = 0; i < n; ++i) vf1[i] *= vf2[i];
    return ApplyInv(n, vf1);
  }

 public:
  static const TSelf& GetFFT() {
    static const TSelf fft;
    return fft;
  }

  static TVector SApply(unsigned n, const TVector& vx) {
    GetFFT().Apply(n, vx);
  }

  static TVector SApplyInv(unsigned n, const TVector& vx) {
    GetFFT().ApplyInv(n, vx);
  }

  static constexpr TVector SConvolution(const TVector& vx) {
    return GetFFT().Convolution(vx);
  }

  static constexpr TVector SConvolution(const TVector& vx1,
                                        const TVector& vx2) {
    return GetFFT().Convolution(vx1, vx2);
  }
};

template <class TModular>
using FFTA =
    FFT<TModular, FactorizeBase(TModular::GetMod() - 1)[0].power,
        FindSmallestPrimitiveRoot<typename TModular::TArithmetic>(
            TModular::GetMod(), FactorizeBase(TModular::GetMod() - 1))>;
}  // namespace mstatic
}  // namespace modular

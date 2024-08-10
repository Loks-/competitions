#pragma once

#include "common/base.h"
#include "common/factorization/utils/factorization_base.h"
#include "common/modular/utils/primitive_root.h"
#include "common/numeric/bits/ulog2.h"

#include <algorithm>
#include <vector>

namespace modular {
namespace mstatic {
template <class TModular, unsigned log2_maxn, unsigned primitive_root>
class FFT {
 public:
  using TVector = std::vector<TModular>;

 protected:
  static constexpr unsigned maxn = (1u << log2_maxn);
  static constexpr TModular primitive = primitive_root;

 protected:
  std::vector<TVector> roots;
  std::vector<std::vector<unsigned>> bit_rev_map;

 protected:
  constexpr void Init() {
    constexpr uint64_t p = TModular::GetMod();
    static_assert(((p - 1) % maxn) == 0);
    static_assert(IsPrimitiveRoot(p, FactorizeBase(p - 1), primitive_root));
    TVector base_roots(maxn);
    TModular nprimitive = primitive.PowU((p - 1) / maxn), r = 1;
    for (unsigned i = 0; i < maxn; ++i) {
      base_roots[i] = r;
      r *= nprimitive;
    }
    assert(r == 1);

    roots.clear();
    roots.reserve(log2_maxn + 1);
    for (unsigned k = 1; k <= maxn; k *= 2) {
      TVector v(k);
      for (unsigned i = 0; i < k; ++i) v[i] = base_roots[i * (maxn / k)];
      roots.push_back(v);
    }

    bit_rev_map.clear();
    bit_rev_map.reserve(log2_maxn + 1);
    bit_rev_map.push_back({0});
    for (unsigned b = 1; b < maxn; b *= 2) {
      std::vector<unsigned> v(2 * b);
      for (unsigned i = 0; i < 2 * b; ++i)
        v[i] = bit_rev_map.back()[i / 2] + ((i & 1) ? b : 0u);
      bit_rev_map.push_back(v);
    }
  }

  constexpr void FFTI_Adjust(TVector& output) const {
    std::reverse(output.begin() + 1, output.end());
    TModular invn = TModular(output.size()).Inverse();
    for (auto& o : output) o *= invn;
  }

 public:
  static constexpr unsigned GetFFTN(unsigned l) {
    unsigned n = 1;
    for (; n < l;) n *= 2;
    return n;
  }

  constexpr FFT() { Init(); }

  static constexpr unsigned GetMaxN() { return maxn; }

  constexpr TVector Apply(unsigned n, const TVector& vx) const {
    assert((n > 0) && (maxn % n == 0));
    const unsigned k = numeric::ULog2(n);
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

  constexpr TVector ApplyI(unsigned n, const TVector& vx) const {
    TVector output = Apply(n, vx);
    FFTI_Adjust(output);
    return output;
  }

  constexpr TVector Convolution(const TVector& vx) {
    const unsigned n = GetFFTN(unsigned(2 * vx.size()));
    auto vf = Apply(n, vx);
    for (auto& f : vf) f *= f;
    return ApplyI(n, vf);
  }

  constexpr TVector Convolution(const TVector& vx1, const TVector& vx2) {
    const unsigned n = GetFFTN(unsigned(vx1.size() + vx2.size()));
    auto vf1 = Apply(n, vx1), vf2 = Apply(n, vx2);
    for (unsigned i = 0; i < n; ++i) vf1[i] *= vf2[i];
    return ApplyI(n, vf1);
  }
};

template <class TModular, unsigned log2_maxn>
using FFTA =
    FFT<TModular, log2_maxn,
        FindSmallestPrimitiveRoot<typename TModular::TArithmetic>(
            TModular::GetMod(), FactorizeBase(TModular::GetMod() - 1))>;
}  // namespace mstatic
}  // namespace modular

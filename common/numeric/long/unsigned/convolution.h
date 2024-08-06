#pragma once

#include "common/base.h"
#include "common/numeric/long/unsigned.h"
#include "common/numeric/long/unsigned/multiplication.h"
#include "common/numeric/long/unsigned/pow.h"

#include <algorithm>
#include <vector>

namespace numeric {
namespace nlong {
namespace hidden {
constexpr Unsigned ConvolutionV2U(const std::vector<Unsigned>& a,
                                  unsigned xsize) {
  Unsigned::TData rd(a.size() * xsize);
  for (unsigned i = 0; i < a.size(); ++i) {
    auto& ai = a[i].Data();
    for (unsigned j = 0; j < ai.size(); ++j) rd[i * xsize + j] = ai[j];
  }
  return Unsigned(rd);
}

constexpr std::vector<Unsigned> ConvolutionU2V(const Unsigned& a,
                                               unsigned xsize) {
  if (!a.Size()) return {};
  auto& ad = a.Data();
  unsigned l = (ad.size() - 1) / xsize + 1;
  std::vector<Unsigned> v(l);
  for (unsigned i = 0; i < l; ++i) {
    v[i] = Unsigned(Unsigned::TData(
        ad.begin() + i * xsize,
        ad.begin() + std::min<unsigned>((i + 1) * xsize, ad.size())));
  }
  return v;
}
}  //  namespace hidden

template <unsigned maxn = (1u << 16)>
inline std::vector<Unsigned> Convolution(const std::vector<Unsigned>& a,
                                         const std::vector<Unsigned>& b) {
  unsigned max_size_a = 0, max_size_b = 0;
  for (auto& ai : a) max_size_a = std::max<unsigned>(max_size_a, ai.Size());
  for (auto& bi : b) max_size_b = std::max<unsigned>(max_size_b, bi.Size());
  if ((max_size_a == 0) || (max_size_b == 0)) return {};
  unsigned x_size = max_size_a + max_size_b + 1;
  assert(x_size * (a.size() + b.size()) <= maxn);
  auto r = Mult<maxn>(hidden::ConvolutionV2U(a, x_size),
                      hidden::ConvolutionV2U(b, x_size));
  auto vr = hidden::ConvolutionU2V(r, x_size);
  assert(vr.size() == a.size() + b.size() - 1);
  return vr;
}

template <unsigned maxn = (1u << 16)>
inline std::vector<Unsigned> ConvolutionN(const std::vector<Unsigned>& a,
                                          unsigned n) {
  unsigned max_size_a = 0;
  for (auto& ai : a) max_size_a = std::max<unsigned>(max_size_a, ai.Size());
  if (max_size_a == 0) return {};
  unsigned x_size = n * max_size_a + n / 16 + 1;
  assert(x_size * a.size() <= maxn);
  auto r = PowU<maxn>(hidden::ConvolutionV2U(a, x_size), n);
  auto vr = hidden::ConvolutionU2V(r, x_size);
  assert(vr.size() == (a.size() - 1) * n + 1);
  return vr;
}
}  // namespace nlong
}  // namespace numeric

#pragma once

#include "common/modular.h"
#include "common/modular/static/convolution_fft.h"
#include "common/numeric/convolution_base.h"

#include <vector>

namespace modular {
namespace mstatic {
template <uint64_t prime>
inline std::vector<TModular_P32<prime>> Convolution(
    const std::vector<TModular_P32<prime>>& a) {
  return (a.size() < 100) ? numeric::ConvolutionBase(a)
                          : ConvolutionFFT<TModular_P32<prime>>::Convolution(a);
}

template <uint64_t prime>
inline std::vector<TModular_P32<prime>> Convolution(
    const std::vector<TModular_P32<prime>>& a,
    const std::vector<TModular_P32<prime>>& b) {
  return ((a.size() < 100) || (b.size() < 100))
             ? numeric::ConvolutionBase(a, b)
             : ConvolutionFFT<TModular_P32<prime>>::Convolution(a, b);
}
}  // namespace mstatic
}  // namespace modular

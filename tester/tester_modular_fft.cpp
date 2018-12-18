#include "common/modular/static/factorial.h"
#include "common/modular/static/fft.h"
#include "common/modular/static/modular.h"

#include <functional>
#include <iostream>
#include <vector>

using TModular = TModular_P32<7340033>;
using TFactorial = ModularFactorial<TModular>;
using TFFT = ModularFFT<TModular>;

bool TestModularFFT() {
  std::hash<unsigned> h;
  TFFT fft;
  // unsigned n = fft.GetMaxN();
  unsigned n = (1u << 16);
  std::vector<TModular> v1(n), v2(n), v3(n);
  for (unsigned i = 0; i < n; ++i) {
    v1[i] = h(i);
    v2[i] = h(i + n);
    v3[i] = v1[i] + v2[i];
  }
  std::vector<TModular> v1_fft = fft.FFT(n, v1), v2_fft = fft.FFT(n, v2),
                        v3_fft = fft.FFT(n, v3);
  for (unsigned i = 0; i < n; ++i) {
    if (v3_fft[i] != v1_fft[i] + v2_fft[i]) {
      std::cout << "FFT is not linear." << std::endl;
      return false;
    }
  }
  if ((v1 != fft.FFTI(n, v1_fft)) || (v2 != fft.FFTI(n, v2_fft)) ||
      (v3 != fft.FFTI(n, v3_fft))) {
    std::cout << "FFTI(FFT(x)) != x." << std::endl;
    return false;
  }

  TFactorial f;
  TModular r = 2, rp = 1;
  std::vector<TModular> vr_fft = fft.FFT(n, {1, r});
  unsigned power = 1000;
  for (TModular& x : vr_fft) x = x.PowU(power);
  std::vector<TModular> vr = fft.FFTI(n, vr_fft);
  for (unsigned i = 0; i < n; ++i) {
    if (vr[i] != f.BinomialCoefficient(power, i) * rp) {
      std::cout << "Incorrect result for (1 + r)^power @ " << i << std::endl;
      return false;
    }
    rp *= r;
  }
  return true;
}

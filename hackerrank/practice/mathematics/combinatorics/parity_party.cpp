// https://www.hackerrank.com/challenges/parity-party

#include "common/modular/static/fft.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_P32<7340033>;
using TFFT = modular::mstatic::FFTA<TModular, 17>;

int main_parity_party() {
  TModular one = 1;
  unsigned n, a, b, c, s, l;
  cin >> n >> a >> b >> c;
  s = a + b + c;
  TFFT fft;
  l = TFFT::GetFFTN(a + b + 1);
  assert(l < (1u << 17));
  vector<TModular> va_fft = fft.Apply(l, {one, -one});
  vector<TModular> vb_fft = fft.Apply(l, {one, one});
  vector<TModular> vf_fft(l);
  for (unsigned i = 0; i < l; ++i)
    vf_fft[i] = va_fft[i].PowU(a) * vb_fft[i].PowU(b);
  vector<TModular> vf = fft.ApplyI(l, vf_fft);
  TModular res = 0;
  for (unsigned i = 0; i <= a + b; ++i)
    res += vf[i] * (TModular(s) - TModular(2 * i)).PowU(n);
  cout << res / TModular(2).PowU(a + b) << endl;
  return 0;
}

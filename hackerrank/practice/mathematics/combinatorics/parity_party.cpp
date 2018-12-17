// https://www.hackerrank.com/challenges/parity-party

#include "common/modular/static/fft.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_P32<7340033>;
using TFFT = ModularFFT<TModular>;

int main_parity_party()
{
    TModular one = 1;
    TFFT fft;
	unsigned n, a, b, c, s, l;
	cin >> n >> a >> b >> c;
    s = a + b + c;
    for (l = 1; l <= a + b; l *= 2);
    vector<TModular> va_fft = fft.FFT(l, {one, -one});
    vector<TModular> vb_fft = fft.FFT(l, {one, one});
    vector<TModular> vf_fft(l);
    for (unsigned i = 0; i < l; ++i)
        vf_fft[i] = va_fft[i].PowU(a) * vb_fft[i].PowU(b);
    vector<TModular> vf = fft.FFTI(l, vf_fft);
	TModular res = 0;
    for (unsigned i = 0; i <= a + b; ++i)
        res += vf[i] * (TModular(s) - TModular(2 * i)).PowU(n);
	cout << res / TModular(2).PowU(a + b) << endl;
	return 0;
}

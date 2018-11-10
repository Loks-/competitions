// https://www.hackerrank.com/challenges/connect-the-country

#include "common/numeric/factorial_double.h"
#include "common/stl/base.h"

int main_connect_the_country()
{
    struct CS
    {
        vector<double> p, sp;
        double get(unsigned j)
        {
            return (j < sp.size()) ? sp[j] : 1.0;
        }
    };

	FactorialDouble f;
	unsigned N;
	cin >> N;
	vector<CS> v;
	v.resize(N + 1);
	for (unsigned i = 0; i <= N; ++i)
	{
		if ((i == 0) || (i == 1))
		{
			v[i].p.resize(1, 1.0);
			v[i].sp.resize(1, 1.0);
		}
		else if (i == 2)
		{
			v[i].p = { 0.0, 1.0 };
			v[i].sp = { 0.0, 1.0 };
		}
		else
		{
			unsigned maxk = ((i - 1) * (i - 2)) / 2 + 1;
			unsigned e = (i * (i - 1)) / 2;
			v[i].p.resize(maxk + 1);
			v[i].sp.resize(maxk + 1);
			for (unsigned j = i - 1; j <= maxk; ++j)
			{
				for (unsigned jl = 0; jl < j; ++jl)
				{
					unsigned jr = j - jl - 1;
					for (unsigned il = 1; il < i; ++il)
					{
						unsigned ir = i - il;
						double pl = v[il].get(jl);
						double pr = v[ir].get(jr);
						if ((pl == 0.) || (pr == 0.))
							continue;
						unsigned el = (il * (il - 1)) / 2;
						unsigned er = (ir * (ir - 1)) / 2;
						if ((jl > el) || (jr > er))
							continue;
						double prec = log(pl) + log(pr);
						double pedges = f.GetLog(e - j) - f.GetLog(e);
						pedges += log(double(il)) + log(double(ir)); // last edge
						pedges += f.BinomialCoefficientLog(j - 1, jl);
						pedges += f.GetLog(el) - f.GetLog(el - jl);
						pedges += f.GetLog(er) - f.GetLog(er - jr);
						double pf = prec + pedges + f.BinomialCoefficientLog(i, il) - log(2.);
						v[i].p[j] += exp(pf);
					}
				}
				v[i].sp[j] = min(v[i].sp[j - 1] + v[i].p[j], 1.0);
			}
		}
	}
	long double ans = 0.;
	for (unsigned j = 0; j < v[N].p.size(); ++j)
		ans += j * v[N].p[j];
	cout << int(ans) << endl;
	return 0;
}

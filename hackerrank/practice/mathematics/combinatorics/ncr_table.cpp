// https://www.hackerrank.com/challenges/ncr-table

#include "common/modular/static/binomial_coefficient_table.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_C32<1000000000>;

int main_ncr_table()
{
    ModularBinomialCoefficientTable<TModular> table;
    unsigned T, n;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> n;
        for (unsigned i = 0; i <= n; ++i)
            cout << table(n, i) << " ";
        cout << endl;
    }
	return 0;
}

// https://www.hackerrank.com/challenges/flip

#include "common/linear_algebra/field/lup_decomposition.h"
#include "common/linear_algebra/matrix.h"
#include "common/linear_algebra/vector.h"
#include "common/modular/modular_io.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

using TModular = Modular<2>;
using TVector = Vector<TModular>;
using TMatrix = Matrix<TModular>;

int main_find_the_operations__lupd()
{
    unsigned N, D;
    cin >> N >> D;
    unsigned NN = N*N;
    TVector v = ReadVector<TModular>(NN), x(NN);
    TMatrix M(NN);
    for (unsigned i = 0; i < NN; ++i)
    {
        int i1 = int(i / N), i2 = int(i % N);
        for (unsigned j = 0; j < NN; ++j)
        {
            int j1 = int(j / N), j2 = int(j % N);
            if (abs(i1 - j1) + abs(i2 - j2) <= int(D))
                M(i, j) = 1;
        }
    }
    FLUPDecomposition<TMatrix> lupd;
    lupd.Build(M);
    if (lupd.Solve(v, x))
    {
        cout << "Possible" << endl;
        unsigned s = 0;
        for (TModular t : x)
            s += t.Get();
        cout << s << endl;
        for (unsigned i = 0; i < NN; ++i)
        {
            if (x(i).Get())
                cout << i / N << " " << i % N << endl;
        }
    }
    else
        cout << "Impossible" << endl;
	return 0;
}

// https://www.hackerrank.com/challenges/colorful-polygon

#include "common/modular/static/fibonacci.h"
#include "common/modular/static/modular_io.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFibonacci = ModularFibonacci<TModular>;

int main_colorful_polygon()
{
    TFibonacci f;
    unsigned n, i, j;
    cin >> n;
    vector<unsigned> v = ReadVector<unsigned>(n);
    TModular r = 1;
    for (i = 0; (i < n) && (v[i] == v[(i + n - 1) % n]);) ++i;
    if (i == n)
    {
        r = 7;
    }
    else
    {
        vector<unsigned> vt(4, 0);
        for (; i < n; i += j)
        {
            for (j = 1; v[(i + j) % n] == v[i];) ++j;
            r *= f(j + 2);
            vt[min(j, 4u) - 1] += 1;
        }
        if ((vt[3] == 0) && (vt[1] + vt[2] <= 1))
        {
            if (vt[1] == 1)
                r -= 2;
            else if (vt[2] == 1)
                r -= 1;
            else // (vt[1] == 0) && (vt[2] == 0)
                r -= (n + 1);
        }
    }
    cout << r << endl;
	return 0;
}

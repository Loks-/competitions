#include "common/vector/utils.h"
#include "common/stl_base.h"

int main_lollipop_shop()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		unsigned N, D;
		cin >> N;
        vector<unsigned> vc(N, 0);
        for (unsigned i = 0; i < N; ++i)
        {
            cin >> D;
            vector<unsigned> v = ReadVector<unsigned>(D);
            unsigned best = N + 1, bestu = N;
            for (unsigned u : v)
            {
                vc[u] += 1;
                if (best > vc[u])
                {
                    best = vc[u];
                    bestu = u;
                }
            }
            if (bestu < N)
            {
                cout << bestu << endl;
                vc[bestu] += N;
            }
            else
                cout << -1 << endl;
        }
	}
	return 0;
}

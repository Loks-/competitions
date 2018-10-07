#include "common/vector/read.h"
#include "common/stl/base.h"

int main_armed_bandit()
{
    unsigned T, N;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> N;
        vector<unsigned> v = ReadVector<unsigned>(N);
        for (unsigned i = 0; i + 1 < v.size(); ++i)
        {
            if (v[i] >= 100)
                cout << "100 ";
            else if (v[i] >= 10)
                cout << "10 ";
            else
                cout << "1 ";
        }
        cout << "1" << endl;
    }
    return 0;
}

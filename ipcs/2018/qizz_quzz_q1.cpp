#include "common/factorization/utils/divisors.h"
#include "common/factorization/primes_list.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

#include <cctype>
#include <string>

int main_qizz_quzz_q1()
{
    PrimesList primes_list(1000);
    auto get_type = [](const std::string& s)
    {
        unsigned type = 0;
        for (char c : s)
        {
            if (isdigit(c))
                type |= 1;
            else if (('a' <= c) && (c <= 'z'))
                type |= 2;
            else
                type |= 4;
        }
        return type;
    };

    unsigned T, N, i;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> N;
        vector<string> vs = ReadVector<string>(N), vkey(N + 1);
        unsigned k = 0;
        for (i = 1; i <= N; ++i)
        {
            string s = vs[i-1];
            unsigned type = get_type(s);
            if ((type != 1) && (type != 2))
                break;
            vector<uint64_t> vd = GetDivisors(primes_list.Factorize(i));
            sort(vd.begin(), vd.end());
            for (uint64_t d : vd)
            {
                if (d == i) continue;
                if (vkey[d] != "")
                {
                    if (s.substr(0, vkey[d].size()) != vkey[d])
                    {
                        type = 0;
                        break;
                    }
                    s = s.substr(vkey[d].size());
                }
            }
            if (type == 0)
                break;
            if (type == 1)
            {
                if (s != std::to_string(i))
                    break;
            }
            else // type == 2
            {
                if (s.size() == 0)
                    continue;
                if (s.size() != 4)
                    break;
                if (++k >= 3)
                    break;
                vkey[i] = s;
            }
        }
        cout << i - 1 << endl;
    }
    return 0;
}

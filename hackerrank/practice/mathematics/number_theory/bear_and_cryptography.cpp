// https://www.hackerrank.com/challenges/bear-and-cryptography

#include "common/numeric/primality_test.h"
#include "common/numeric/primes_list.h"
#include "common/stl_base.h"

namespace {
    static PrimesList pl(10000000);
    static MillerRabinPrimalityTest pt;
    static uint64_t best_so_far;

    uint64_t GetMaxPrimeLessOrEqualN(uint64_t N, uint64_t minN)
    {
        if (N <= 1)
        {
            return 0;
        }
        else if (N <= pl.vprimes.back())
        {
            return *(upper_bound(pl.vprimes.begin(), pl.vprimes.end(), N) - 1);
        }
        else
        {
            if ((N & 1) == 0)
                N -= 1;
            for (; N > minN; N -= 2)
            {
                if (pt.IsPrime(N))
                    return N;
            }
            return 1; // Not Zero!
        }
    }

    // True - solutions are possible, just impossibe to beat best one
    // False - primes too big already
    bool Solve(uint64_t N, uint64_t M, const vector<unsigned>& powers, unsigned power_index, unsigned min_prime_index)
    {
        if (power_index == powers.size())
        {
            if (N < 1)
            {
                return false;
            }
            else
            {
                best_so_far = max(best_so_far, M);
                return true;
            }
        }
        else if (N < pl.vprimes[min_prime_index])
        {
            return false;
        }
        else if ((powers[power_index] >= 3) && (N < pl.vprimes2[min_prime_index]))
        {
            return false;
        }
        else if ((power_index == (powers.size() - 1)) && powers[power_index] <= 3)
        {
            if (powers[power_index] == 2)
            {
                uint64_t p = GetMaxPrimeLessOrEqualN(N, best_so_far / M);
                best_so_far = max(best_so_far, M * p);
                return true;
            }
            else if (powers[power_index] == 3)
            {
                uint64_t p2 = *(upper_bound(pl.vprimes2.begin(), pl.vprimes2.end(), N) - 1);
                best_so_far = max(best_so_far, M * p2);
                return true;
            }
        }
        bool first = true;
        for (;; ++min_prime_index)
        {
            unsigned power = powers[power_index] - 1;
            uint64_t prime = pl.vprimes[min_prime_index];
            uint64_t tx = 1, tn = N;
            for (unsigned i = 0; i < power; ++i)
            {
                tx *= prime;
                tn /= prime;
            }
            bool b = Solve(tn, M*tx, powers, power_index + 1, min_prime_index + 1);
            if (first && !b)
                return false;
            first = false;
            if (M*N <= best_so_far)
                break;
            if (!b)
                break;
        }
        return true;
    }

    void Solve(uint64_t N, const vector<unsigned>& powers)
    {
        Solve(N, 1, powers, 0, 0);
    }

    vector<vector<unsigned>> DivideK(unsigned K, unsigned minv = 2)
    {
        if (K == 1)
        {
            return vector<vector<unsigned>>(1);
        }
        else if (K < minv)
        {
            return vector<vector<unsigned>>();
        }
        vector<vector<unsigned>> res;
        for (unsigned a = minv; a <= K; ++a)
        {
            if ((K % a) == 0)
            {
                vector<vector<unsigned>> v = DivideK(K / a, a);
                for (unsigned i = 0; i < v.size(); ++i)
                {
                    v[i].push_back(a);
                    res.push_back(v[i]);
                }
            }
        }
        return res;
    }

    vector<vector<unsigned>> GetAllPowersCandidates(unsigned K)
    {
        vector<vector<unsigned>> vv = DivideK(K);
        vector<vector<unsigned>>  answer;
        for (size_t i = 0; i < vv.size(); ++i)
        {
            reverse(vv[i].begin(), vv[i].end());
            answer.push_back(vv[i]);
            while (next_permutation(vv[i].begin(), vv[i].end()))
                answer.push_back(vv[i]);
        }
        return answer;
    }
}

int main_bear_and_cryptography()
{
	vector<vector<vector<unsigned>>> candidates(1);
	for (unsigned i = 1; i <= 40; ++i)
	{
		candidates.push_back(GetAllPowersCandidates(i));
	}
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t N;
		unsigned K;
		cin >> N >> K;
		if (K == 1)
		{
			cout << 1 << endl;
			continue;
		}
		best_so_far = 0;
		for (unsigned i = 0; i < candidates[K].size(); ++i)
		{
			Solve(N, candidates[K][i]);
		}
		if (best_so_far == 0)
			best_so_far -= 1;
		cout << int64_t(best_so_far) << endl;
	}
	return 0;
}

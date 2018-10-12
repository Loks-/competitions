// https://www.hackerrank.com/challenges/primitive-problem

#include "common/factorization/utils/eulers_totient.h"
#include "common/factorization/primes_list.h"
#include "common/modular/utils/primitive_root.h"
#include "common/stl/base.h"

int main_primitive_problem()
{
    PrimesList primes_list(100000);
	uint64_t p;
	cin >> p;
    uint64_t u = FindSmallestPrimitive(p, primes_list);
    cout << u << " " << EulersTotient(p - 1, primes_list.Factorize(p - 1)) << endl;
	return 0;
}

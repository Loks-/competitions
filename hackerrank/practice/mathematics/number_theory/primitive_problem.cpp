// https://www.hackerrank.com/challenges/primitive-problem

#include "common/base.h"
#include "common/numeric/primes_list.h"
#include "common/numeric/utils/primitive_root.h"

int main_primitive_problem()
{
    PrimesList pl(100000);
	uint64_t p;
	cin >> p;
    uint64_t u = FindSmallestPrimitive(p, pl);
    cout << u << " " << pl.EulerPhi(p - 1) << endl;
	return 0;
}

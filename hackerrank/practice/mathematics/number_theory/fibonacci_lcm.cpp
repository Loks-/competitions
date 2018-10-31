// https://www.hackerrank.com/challenges/fibonacci-lcm

#include "common/modular/fibonacci_modular_matrix.h"
#include "common/modular/modular_io.h"
#include "common/numeric/utils/gcd.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

#include <unordered_map>

using TModular = Modular<>;
using TFibonacci = FibonacciModularMatrix<TModular>;

int main_fibonacci_lcm()
{
	unsigned N;
	cin >> N;
	vector<uint64_t> va = ReadVector<uint64_t>(N);
	unordered_map<uint64_t, int64_t> current, next;
	for (uint64_t a : va)
	{
		next = current;
		next[a] += 1;
		for (auto p : current)
			next[GCD(a, p.first)] -= p.second;
		current.swap(next);
	}
	TModular r(1);
	for (auto p : current)
		r *= TFibonacci::GetU(p.first).PowS(p.second);
	cout << r << endl;
	return 0;
}

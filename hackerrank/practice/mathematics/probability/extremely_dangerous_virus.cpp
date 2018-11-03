// https://www.hackerrank.com/challenges/extremely-dangerous-virus

#include "common/modular/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_extremely_dangerous_virus()
{
	TModular a, b;
	uint64_t t;
	cin >> a >> b >> t;
	cout << ((a + b) / 2).PowU(t) << endl;
	return 0;
}

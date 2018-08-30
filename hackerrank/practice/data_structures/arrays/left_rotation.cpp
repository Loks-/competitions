// https://www.hackerrank.com/challenges/array-left-rotation/

#include "common/stl_base.h"

int main_left_rotation()
{
	unsigned n, d;
	cin >> n >> d;
	vector<unsigned> v(n);
	for (unsigned i = 0; i < n; ++i)
		cin >> v[(i + n - d) % n];
	for (unsigned i : v)
		cout << i << " ";
	cout << endl;
	return 0;
}

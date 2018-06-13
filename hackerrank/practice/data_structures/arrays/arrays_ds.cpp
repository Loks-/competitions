// https://www.hackerrank.com/challenges/arrays-ds

#include "common/base.h"

int main_arrays_ds()
{
	unsigned n;
	cin >> n;
	vector<int> v(n);
	for (unsigned i = 0; i < n; ++i)
		cin >> v[i];
	reverse(v.begin(), v.end());
	for (unsigned j : v)
		cout << j << " ";
	cout << endl;
	return 0;
}

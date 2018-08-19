// https://www.hackerrank.com/challenges/arrays-ds

#include "common/vector/utils.h"
#include "common/stl_base.h"

int main_arrays_ds()
{
	unsigned n;
	cin >> n;
	vector<int> v = ReadVector<int>(n);
	reverse(v.begin(), v.end());
	for (unsigned j : v)
		cout << j << " ";
	cout << endl;
	return 0;
}

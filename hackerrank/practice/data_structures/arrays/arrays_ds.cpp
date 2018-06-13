// https://www.hackerrank.com/challenges/arrays-ds

#include "common/base.h"
#include "common/vector/utils.h"

int main_arrays_ds()
{
	unsigned n;
	cin >> n;
	vector<int> v = VectorUtils::ReadVector<int>(n);
	reverse(v.begin(), v.end());
	for (unsigned j : v)
		cout << j << " ";
	cout << endl;
	return 0;
}

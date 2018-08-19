// https://www.hackerrank.com/challenges/maximum-element

#include "common/stl_base.h"

#include <stack>

int main_maximum_element()
{
	unsigned N, x, t;
	cin >> N;
	stack<unsigned> s;
	for (unsigned it = 0; it < N; ++it)
	{
		cin >> t;
		if (t == 1)
		{
			cin >> x;
			if (!s.empty())
				x = max(x, s.top());
			s.push(x);
		}
		else if (t == 2)
			s.pop();
		else if (t == 3)
			cout << s.top() << endl;
	}
	return 0;
}

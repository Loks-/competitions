#include "common/stl_base.h"

#include <queue>

int main_senate_evacuation()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		unsigned N, t, s = 0;
		cin >> N;
		priority_queue<pair<unsigned, char>> pq;
		for (unsigned i = 0; i < N; ++i)
		{
			cin >> t;
			s += t;
			if (t > 0)
				pq.push(make_pair(t, char(i)));
		}
		cout << "Case #" << it << ": ";
		if (s & 1)
		{
			auto p = pq.top(); pq.pop();
			printf("%c ", p.second + 'A');
			// cout << (p.second + 'A') << " ";
			p.first -= 1;
			if (p.first > 0)
				pq.push(p);
		}
		for (;;)
		{
			for (unsigned i = 0; i < 2; ++i)
			{
				auto p = pq.top(); pq.pop();
				printf("%c", p.second + 'A');
				// cout << (p.second + 'A');
				p.first -= 1;
				if (p.first > 0)
					pq.push(p);
			}
			if (pq.empty())
				break;
			cout << " ";
		}
		cout << endl;
	}
	return 0;
}

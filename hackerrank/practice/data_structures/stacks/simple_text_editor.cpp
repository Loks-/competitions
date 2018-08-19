// https://www.hackerrank.com/challenges/simple-text-editor

#include "common/stl_base.h"

#include <stack>
#include <string>

int main_simple_text_editor()
{
	unsigned T, type, k;
	cin >> T;
	stack<pair<int, string>> st;
	string scurrent, stemp;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> type;
		if (type == 1)
		{
			cin >> stemp;
			scurrent += stemp;
			st.push({ 1, stemp });
		}
		else if (type == 2)
		{
			cin >> k;
			stemp = scurrent.substr(scurrent.size() - k);
			scurrent.resize(scurrent.size() - k);
			st.push({ 2, stemp });
		}
		else if (type == 3)
		{
			cin >> k;
			assert(k <= scurrent.size());
			cout << scurrent[k - 1] << endl;
		}
		else if (type == 4)
		{
			if (st.top().first == 1)
				scurrent.resize(scurrent.size() - st.top().second.size());
			else
				scurrent += st.top().second;
			st.pop();
		}
		else
			assert(false);
	}
	return 0;
}

// https://www.hackerrank.com/challenges/balanced-brackets

#include "common/stl_base.h"

#include <stack>
#include <string>
#include <unordered_map>

namespace {
	unordered_map<char, char> m;

	bool Correct(const string& s)
	{
		stack<char> st;
		for (char c : s)
		{
			auto it = m.find(c);
			if (it == m.end())
				st.push(c);
			else
			{
				if (st.empty())
					return false;
				if (st.top() != it->second)
					return false;
				st.pop();
			}
		}
		return st.empty();
	}
}

int main_balanced_brackets()
{
	m[')'] = '(';
	m[']'] = '[';
	m['}'] = '{';
	int T;
	cin >> T;
	for (; T; --T)
	{
		string s;
		cin >> s;
		if (Correct(s))
			cout << "YES" << endl;
		else
			cout << "NO" << endl;
	}
	return 0;
}

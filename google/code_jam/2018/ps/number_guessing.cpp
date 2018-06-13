#include "common/base.h"

int main_number_guessing()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		unsigned A, B, N;
		cin >> A >> B >> N;
		string s;
		for (bool found = false; !found;)
		{
			unsigned q = (A + B) / 2 + 1;
			cout << q << endl;
			cin >> s;
			if (s == "CORRECT")
				found = true;
			else if (s == "TOO_SMALL")
				A = q;
			else if (s == "TOO_BIG")
				B = q - 1;
			else
				assert(false);
		}
	}
	return 0;
}

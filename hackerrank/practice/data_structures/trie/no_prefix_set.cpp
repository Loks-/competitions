// https://www.hackerrank.com/challenges/no-prefix-set

#include "common/hash.h"
#include "common/stl_base.h"

#include <unordered_set>
#include <string>

int main_no_prefix_set()
{
    unsigned N;
    cin >> N;
    unordered_set<size_t> sprefix, sstring;
    hash<char> h;
    bool found = false;
    for (unsigned i = 0; i < N; ++i)
    {
        string s;
        cin >> s;
        size_t hvalue = 0;
        for (char c : s)
            hvalue = hash_combine(hvalue, h(c));
        if (sprefix.find(hvalue) != sprefix.end())
            found = true;
        hvalue = 0;
        for (char c : s)
        {
            hvalue = hash_combine(hvalue, h(c));
            if (sstring.find(hvalue) != sstring.end())
                found = true;
            sprefix.insert(hvalue);
        }
        sstring.insert(hvalue);
        if (found)
        {
            cout << "BAD SET" << endl << s << endl;
            break;
        }
    }
    if (!found)
        cout << "GOOD SET" << endl;
	return 0;
}

// https://www.hackerrank.com/challenges/jim-and-the-skyscrapers

#include "common/stl/base.h"

#include <stack>

int main_jim_and_the_skyscrapers()
{
    unsigned N, h;
    uint64_t total = 0;
    stack<pair<unsigned, unsigned>> s;
    cin >> N;
    for (unsigned i = 0; i < N; ++i)
    {
        cin >> h;
        for (; !s.empty() && (s.top().first < h); s.pop());
        if (!s.empty() && (s.top().first == h))
            s.top().second += 1;
        else
            s.push({h, 1});
        total += s.top().second - 1;
    }
    cout << 2 * total << endl;
	return 0;
}

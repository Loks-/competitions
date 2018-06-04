#include <iostream>

using namespace std;

int main()
{
    unsigned n;
    cin >> n;
    for (unsigned i = 0; i < n; ++i)
    {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        cout << 2 * c - a << " " << 2 * d - b << endl;
    }
    return 0;
}

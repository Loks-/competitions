#include "common/heap.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_pottery_lottery() {
  unsigned V = 14, N = 60;
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned day, k, bet;
    HeapMinOnTop<pair<unsigned, unsigned>> h;
    vector<unsigned> vmask(21, 0);
    for (unsigned j = 0; j < 100; ++j) {
      cin >> day;
      if (j < N) {
        cout << (j % V) + 1 << " " << j + 1 << endl;
      } else if (j < N + 20) {
        cout << j - N + 1 << " " << 0 << endl;
        cin >> k;
        nvector::Read<unsigned>(k);
        h.Add({k, j - N + 1});
        if (j == N + 19) {
          bet = h.Extract().second;
        }
      } else if (j < 99) {
        auto p = h.Extract();
        cout << p.second << " " << (vmask[p.second] ? j + 1 : 100) << endl;
        vmask[p.second] = 1;
        h.Add({p.first + 1, p.second});
      } else {
        cout << bet << " " << 100 << endl;
      }
    }
  }
  return 0;
}

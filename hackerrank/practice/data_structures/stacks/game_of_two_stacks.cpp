// https://www.hackerrank.com/challenges/game-of-two-stacks

#include "common/stl/base.h"
#include "common/vector/read.h"

int main_game_of_two_stacks() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned n, m, x, best = 0;
    cin >> n >> m >> x;
    vector<unsigned> va = nvector::Read<unsigned>(n);
    va.push_back(x + 1);
    vector<unsigned> vb = nvector::Read<unsigned>(m);
    vb.push_back(x + 1);
    unsigned i = 0, j = 0, s = 0;
    for (; s <= x;) {
      best = i;
      s += va[i++];
    }
    for (; i > 0;) {
      if (s <= x) {
        best = max(best, i + j);
        s += vb[j++];
      } else {
        s -= va[--i];
      }
    }
    for (; s <= x;) {
      best = max(best, i + j);
      s += vb[j++];
    }
    cout << best << endl;
  }
  return 0;
}

// https://www.hackerrank.com/challenges/queue-using-two-stacks

#include "common/stl/base.h"

#include <queue>

int main_queue_using_two_stacks() {
  unsigned Q, type;
  int x;
  queue<int> q;
  cin >> Q;
  for (unsigned iq = 0; iq < Q; ++iq) {
    cin >> type;
    if (type == 1) {
      cin >> x;
      q.push(x);
    } else if (type == 2) {
      q.pop();
    } else if (type == 3) {
      cout << q.front() << endl;
    } else {
      assert(false);
    }
  }
  return 0;
}

#include "pancakes.h"
#include "message.h"

#include "common/stl/base.h"

namespace {
void solve_small_pancakes() {
  int64_t last = GetNumDiners() + 1;
  int64_t l = GetStackSize();
  int64_t s = 0;
  for (int64_t i = 0; i < l; ++i) {
    int64_t current = GetStackItem(i);
    if (current < last) s += 1;
    last = current;
  }
  cout << s << endl;
}
}  // namespace

int main_pancakes() {
  int nodes = NumberOfNodes();
  int node_id = MyNodeId();
  int64_t length = GetStackSize();
  if ((nodes == 1) || (length <= 2 * nodes)) {
    if (node_id == 0) solve_small_pancakes();
    return 0;
  }

  int64_t ifirst = node_id * length / nodes;
  int64_t ilast = (node_id + 1) * length / nodes;
  assert(ilast - ifirst >= 2);
  int64_t h0 = GetStackItem(ifirst);
  int64_t last = h0, s = 0;
  for (int64_t i = ifirst + 1; i < ilast; ++i) {
    int64_t current = GetStackItem(i);
    if (current < last) s += 1;
    last = current;
  }
  if (node_id == 0) {
    s += 1;
  } else {
    Receive(node_id - 1);
    int64_t ls = GetLL(node_id - 1);
    int64_t llast = GetLL(node_id - 1);
    s += ls;
    if (llast > h0) s += 1;
  }
  if (node_id != nodes - 1) {
    PutLL(node_id + 1, s);
    PutLL(node_id + 1, last);
    Send(node_id + 1);
  } else {
    cout << s << endl;
  }
  return 0;
}

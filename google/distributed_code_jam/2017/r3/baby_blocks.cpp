#include "baby_blocks.h"
#include "message.h"

#include "common/stl/base.h"

namespace {
void get_blocks_sum() {
  int64_t nodes = NumberOfNodes();
  int64_t node_id = MyNodeId();
  int64_t length = GetNumberOfBlocks();
  int64_t first = node_id * length / nodes;
  int64_t last = (node_id + 1) * length / nodes;
  int64_t s = 0;
  for (int64_t i = first; i < last; ++i) s += GetBlockWeight(i);
  for (int i = 0; i < nodes; ++i) {
    PutLL(i, s);
    Send(i);
  }
}

int64_t solve(int64_t b1, int64_t b2, int64_t nblocks,
              const vector<int64_t>& vs) {
  int64_t length = GetNumberOfBlocks();
  int64_t i1b = b1 * length / nblocks;
  int64_t i1e = (b1 + 1) * length / nblocks;
  int64_t i2b = b2 * length / nblocks;
  int64_t i2e = (b2 + 1) * length / nblocks;
  int64_t s1 = vs[b1], s2 = vs[nblocks] - vs[b2 + 1];
  int64_t i1 = i1b, i2 = i2e;
  int64_t count = 0;
  for (; (i1 <= i2) && (i1 < i1e) && (i2b < i2);) {
    if (s1 < s2) {
      s1 += GetBlockWeight(i1++);
    } else if (s1 > s2) {
      s2 += GetBlockWeight(--i2);
    } else {
      count += 1;
      s1 += GetBlockWeight(i1++);
      s2 += GetBlockWeight(--i2);
    }
  }
  return count;
}

void solve_small_baby_blocks() {
  int node_id = MyNodeId();
  if (node_id != 0) return;
  int nblocks = int(GetNumberOfBlocks());
  int i1 = 0, i2 = nblocks, k = -1;
  int64_t s1 = 0, s2 = 0;
  for (; i1 <= i2;) {
    if (s1 < s2) {
      s1 += GetBlockWeight(i1++);
    } else if (s1 > s2) {
      s2 += GetBlockWeight(--i2);
    } else {
      ++k;
      s1 += GetBlockWeight(i1++);
      s2 += GetBlockWeight(--i2);
    }
  }
  cout << k << endl;
}
}  // namespace

int main_baby_blocks() {
  int nodes = NumberOfNodes();
  int node_id = MyNodeId();
  if (GetNumberOfBlocks() <= 10 * nodes) {
    solve_small_baby_blocks();
    return 0;
  }
  get_blocks_sum();
  vector<int64_t> vr(nodes), vs(nodes + 1);
  vs[0] = 0;
  for (int i = 0; i < nodes; ++i) {
    Receive(i);
    vr[i] = GetLL(i);
    vs[i + 1] = vs[i] + vr[i];
  }
  int64_t sum = vs.back(), s = 0;
  int k = 0, i1 = 0, i2 = nodes - 1;
  for (; i1 <= i2; ++k) {
    if ((k % nodes) == node_id) s += solve(i1, i2, nodes, vs);
    if (vs[i1 + 1] + vs[i2] > sum) {
      --i2;
    } else if (vs[i1 + 1] + vs[i2] < sum) {
      ++i1;
    } else {
      // if ((i1 != i2) && (node_id == 0))
      //	s += 1;
      ++i1;
      --i2;
    }
  }
  if (node_id == 0) {
    if (vs[i1] * 2 == sum) s += 1;
    for (int i = 1; i < nodes; ++i) {
      Receive(i);
      s += GetLL(i);
    }
    cout << s - 1 << endl;
  } else {
    PutLL(0, s);
    Send(0);
  }
  return 0;
}

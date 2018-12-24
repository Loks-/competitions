#include "broken_memory.h"
#include "message.h"

#include "common/modular/static/modular.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_broken_memory() {
  int nodes = NumberOfNodes();
  int node_id = MyNodeId();
  int node_id2 = node_id ^ 1;
  int length = int(GetLength());

  vector<uint64_t> values(length);
  for (int i = 0; i < length; ++i) {
    values[i] = uint64_t(GetValue(i));
  }

  const TModular c = 195731;
  auto h = [&](int f, int l) {
    TModular v = 0;
    for (int i = f; i < l; ++i) {
      v *= c;
      v += TModular(values[i]);
    }
    return int(v.Get());
  };

  int f1 = 0, f2 = 0, l1 = length, l2 = length, f, l;
  for (; (l1 > f1 + 1) || (l2 > f2 + 1);) {
    if (l1 > f1 + 1) {
      f = f1;
      l = l1;
    } else {
      f = f2;
      l = l2;
    }
    int m = (f + l) / 2;
    int x11 = h(f, m), x12 = h(m, l);
    PutInt(node_id2, x11);
    PutInt(node_id2, x12);
    Send(node_id2);
    Receive(node_id2);
    int x21 = GetInt(node_id2);
    int x22 = GetInt(node_id2);
    if (x11 != x21) {
      if (x12 != x22) {
        assert((f1 == f2) && (l1 == l2));
        l1 = m;
        f2 = m;
      } else {
        if (f1 == f2)
          l1 = l2 = m;
        else if (f1 == f)
          l1 = m;
        else
          l2 = m;
      }
    } else {
      if (x12 != x22) {
        if (f1 == f2)
          f1 = f2 = m;
        else if (f1 == f)
          f1 = m;
        else
          f2 = m;
      } else {
        assert(false);
      }
    }
  }
  assert(f1 != f2);

  if (node_id & 1) {
    // Request
    node_id2 = (node_id + 5) % nodes;
    PutInt(node_id2, f1);
    PutInt(node_id2, f2);
    Send(node_id2);
    Receive(node_id2);
    uint64_t x1 = uint64_t(GetLL(node_id2));
    uint64_t x2 = uint64_t(GetLL(node_id2));
    assert((x1 == values[f1]) != (x2 == values[f2]));
    if (x1 != values[f1]) swap(f1, f2);
    PutInt(0, f1);
    PutInt(0, f2);
    Send(0);
  } else {
    // Support
    node_id2 = (node_id + nodes - 5) % nodes;
    Receive(node_id2);
    f1 = GetInt(node_id2);
    f2 = GetInt(node_id2);
    PutLL(node_id2, values[f1]);
    PutLL(node_id2, values[f2]);
    Send(node_id2);
  }

  if (node_id == 0) {
    for (int inode = 1; inode < nodes; inode += 2) {
      Receive(inode);
      f1 = GetInt(inode);
      f2 = GetInt(inode);
      cout << f1 << " " << f2 << " ";
    }
    cout << endl;
  }

  return 0;
}

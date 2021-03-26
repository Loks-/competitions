#include "lemming.h"
#include "message.h"

#include "common/data_structures/disjoint_set.h"
#include "common/stl/base.h"

#include <unordered_map>

int main_lemming() {
  int nodes = NumberOfNodes();
  int node_id = MyNodeId();
  int64_t rows = GetRows();
  unsigned columns = unsigned(GetColumns());
  if (rows <= 2 * nodes) {
    if (node_id == 0)
      nodes = 1;
    else
      return 0;
  }

  int64_t ifirst = node_id * rows / nodes;
  int64_t ilast = (node_id + 1) * rows / nodes;
  assert(ilast - ifirst >= 1);

  ds::DisjointSet djs(unsigned((ilast - ifirst + 1) * columns));
  unsigned index = 0;
  for (int64_t i = ifirst; i < ilast; ++i) {
    for (unsigned j = 0; j < columns; ++j, ++index) {
      char c = GetDirection(i, j);
      if (c == '^') {
        if (i != ifirst) djs.Union(index, index - columns);
      } else if (c == '>') {
        if (j + 1 != columns) djs.Union(index, index + 1);
      } else if (c == 'v') {
        if (i + 1 != ilast) djs.Union(index, index + columns);
      } else if (c == '<') {
        if (j != 0) djs.Union(index, index - 1);
      } else
        assert(false);
    }
  }

  unsigned total_unions = 0;
  if (node_id != 0) {
    assert(ifirst > 0);
    Receive(node_id - 1);
    total_unions = GetInt(node_id - 1);
    for (unsigned j = 0; j < columns; ++j) {
      index = unsigned(GetInt(node_id - 1) + (ilast - ifirst) * columns);
      if ((GetDirection(ifirst - 1, j) == 'v') ||
          (GetDirection(ifirst, j) == '^'))
        djs.Union(index, j);
    }
  }
  total_unions += djs.GetUnions();

  if (node_id != nodes - 1) {
    PutInt(node_id + 1, total_unions);
    unordered_map<unsigned, unsigned> m;
    unsigned l = 0, shift = unsigned((ilast - ifirst - 1) * columns);
    for (unsigned j = 0; j < columns; ++j) {
      index = djs.Find(shift + j);
      auto it = m.find(index);
      if (it == m.end()) m[index] = l++;
      PutInt(node_id + 1, m[index]);
    }
    Send(node_id + 1);
  } else {
    cout << rows * columns - total_unions << endl;
  }
  return 0;
}

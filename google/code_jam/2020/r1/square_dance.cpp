#include "common/cover/dlvmatrix.h"
#include "common/stl/full.h"
#include "common/unsigned_set.h"

using DLMatrix = cover::DLVMatrix<unsigned>;
using Node = DLMatrix::Node;

int main_square_dance() {
  unsigned T, R, C, s;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> R >> C;
    DLMatrix m(R, C, 0);
    std::vector<Node*> nodes;
    uint64_t score_round = 0, score_total = 0;
    for (unsigned i = 0; i < R; ++i) {
      for (unsigned j = 0; j < C; ++j) {
        cin >> s;
        nodes.push_back(m.Add(i, j, s));
        score_round += s;
      }
    }
    UnsignedSet this_round(nodes.size()), next_round(nodes.size());
    for (unsigned i = 0; i < nodes.size(); ++i) next_round.Insert(i);

    std::vector<Node*> neighbors;
    auto UpdateNeigbors = [&](Node* pnode) {
      neighbors.clear();
      if (!m.IsHeader(pnode->r)) neighbors.push_back(pnode->r);
      if (!m.IsHeader(pnode->d)) neighbors.push_back(pnode->d);
      if (!m.IsHeader(pnode->l)) neighbors.push_back(pnode->l);
      if (!m.IsHeader(pnode->u)) neighbors.push_back(pnode->u);
    };

    for (bool last_round = false; !last_round;) {
      last_round = true;
      score_total += score_round;
      this_round.Swap(next_round);
      next_round.Clear();
      for (unsigned u : this_round.List()) {
        Node* p = nodes[u];
        if (!m.IsNodeEnabled(p)) continue;
        UpdateNeigbors(p);
        unsigned sn = 0;
        for (auto pn : neighbors) sn += pn->value;
        if (neighbors.size() * p->value < sn)
          next_round.Insert(p->row * C + p->column);
      }
      this_round.Swap(next_round);
      next_round.Clear();
      for (unsigned u : this_round.List()) {
        Node* p = nodes[u];
        UpdateNeigbors(p);
        score_round -= p->value;
        m.DisableNode(p);
        for (auto pn : neighbors) next_round.Insert(pn->row * C + pn->column);
        last_round = false;
      }
    }
    cout << "Case #" << it << ": " << score_total << endl;
  }
  return 0;
}

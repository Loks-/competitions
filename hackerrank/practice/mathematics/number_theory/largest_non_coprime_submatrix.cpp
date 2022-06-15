// https://www.hackerrank.com/challenges/largest-coprime-submatrix

#include "common/data_structures/segment_tree/base/get_segment_info.h"
#include "common/data_structures/segment_tree/info/gcd.h"
#include "common/data_structures/segment_tree/segment_tree.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TTree =
    ds::st::SegmentTree<unsigned, ds::st::info::GCD<unsigned>,
                        ds::st::action::None, ds::st::sinfo::Position<>, false>;
using TNode = TTree::TNode;
using TInfo = TNode::TInfo;

int main_largest_non_coprime_submatrix() {
  unsigned n, m, best = 0;
  cin >> n >> m;
  vector<vector<unsigned>> matrix(n);
  for (unsigned i = 0; i < n; ++i) matrix[i] = nvector::Read<unsigned>(m);
  vector<unsigned> vrow(m);
  TTree st(m);
  for (unsigned i1 = 0; i1 < n; ++i1) {
    fill(vrow.begin(), vrow.end(), 0);
    for (unsigned i2 = i1; i2 < n; ++i2) {
      for (unsigned j = 0; j < m; ++j) vrow[j] = GCD(matrix[i2][j], vrow[j]);
      TNode* root = st.BuildTree(vrow);
      for (unsigned j1 = 0, j2 = 1; j1 < m;) {
        unsigned gcd = ds::st::GetSegmentInfo(root, j1, j2).gcd;
        if (gcd > 1) {
          best = max(best, (i2 - i1 + 1) * (j2 - j1));
          if (j2 < m)
            ++j2;
          else
            break;
        } else {
          ++j1;
          j2 = max(j2, j1 + 1);
        }
      }
      st.ResetNodes();
    }
  }
  cout << best << endl;
  return 0;
}

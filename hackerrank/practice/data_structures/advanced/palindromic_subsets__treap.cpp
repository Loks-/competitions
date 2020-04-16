// https://www.hackerrank.com/challenges/palindromic-subsets

#include "common/binary_search_tree/action/rotate_vector__sum.h"
#include "common/binary_search_tree/info/sum.h"
#include "common/binary_search_tree/treap.h"
#include "common/binary_search_tree/utils/add_action_to_segment.h"
#include "common/binary_search_tree/utils/segment_info_by_index.h"
#include "common/linear_algebra/vector_static_size.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

#include <string>

using TVector = la::VectorStaticSize<unsigned, 26>;
using TTree =
    bst::Treap<false, false, TVector, bst::info::Sum<TVector, bst::info::Size>,
               bst::action::RotateVectorSum>;
using TNode = TTree::TNode;

int main_palindromic_subsets__treap() {
  unsigned n, q;
  cin >> n >> q;
  string s;
  cin >> s;
  vector<TVector> vs(n);
  for (unsigned i = 0; i < n; ++i) vs[i](s[i] - 'a') = 1;
  TTree tree(n);
  TNode* root = tree.Build(vs);
  for (unsigned i = 0; i < q; ++i) {
    unsigned t, a, b, c;
    cin >> t;
    if (t == 1) {
      cin >> a >> b >> c;
      root = bst::AddActionToSegment<TTree>(root, a, b + 1, c);
    } else if (t == 2) {
      cin >> a >> b;
      TTree::TInfo info;
      root = bst::SegmentInfoByIndex<TTree>(root, a, b + 1, info);
      unsigned nsum = 0, nnonzero = 0;
      for (unsigned j = 0; j < 26; ++j) {
        if (info.sum(j)) {
          nsum += info.sum(j);
          nnonzero += 1;
        }
      }
      TModularD two = 2;
      cout << (TModularD(nnonzero + 1) * two.PowU(nsum - nnonzero) - 1) << endl;
    }
  }
  return 0;
}

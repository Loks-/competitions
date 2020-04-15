// https://www.hackerrank.com/challenges/palindromic-subsets

#include "common/linear_algebra/vector_static_size.h"
#include "common/modular_io.h"
#include "common/segment_tree/action/rotate_vector.h"
#include "common/segment_tree/base/add_action_to_segment.h"
#include "common/segment_tree/base/get_segment_info.h"
#include "common/segment_tree/info/position.h"
#include "common/segment_tree/info/sum.h"
#include "common/segment_tree/segment_tree.h"
#include "common/stl/base.h"

#include <string>

using TVector = la::VectorStaticSize<unsigned, 26>;
using TTree =
    st::SegmentTree<TVector, st::info::Sum<TVector, st::info::Position<>>,
                    st::action::RotateVector, false>;
using TNode = TTree::TNode;

int main_palindromic_subsets__segment_tree() {
  unsigned n, q;
  cin >> n >> q;
  string s;
  cin >> s;
  vector<TVector> vs(n);
  for (unsigned i = 0; i < n; ++i) vs[i](s[i] - 'a') = 1;
  TTree tree(n);
  TNode* root = tree.BuildTree(vs);
  for (unsigned i = 0; i < q; ++i) {
    unsigned t, a, b, c;
    cin >> t;
    if (t == 1) {
      cin >> a >> b >> c;
      st::AddActionToSegment<TNode>(root, a, b, c);
    } else if (t == 2) {
      cin >> a >> b;
      TTree::TInfo info = st::GetSegmentInfo<TNode>(root, a, b);
      unsigned nsum = 0, nnonzero = 0;
      for (unsigned j = 0; j < 26; ++j) {
        if (info.segment_sum(j)) {
          nsum += info.segment_sum(j);
          nnonzero += 1;
        }
      }
      TModularD two = 2;
      cout << (TModularD(nnonzero + 1) * two.PowU(nsum - nnonzero) - 1) << endl;
    }
  }
  return 0;
}

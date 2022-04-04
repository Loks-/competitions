#include "tester/tester_binary_search_tree_split_join.h"

// #include "common/binary_search_tree/avl_tree.h"
// #include "common/binary_search_tree/perfect_tree.h"
// #include "common/binary_search_tree/red_black_tree.h"
// #include "common/binary_search_tree/scapegoat_tree.h"
// #include "common/binary_search_tree/wavl_tree.h"

#include "common/binary_search_tree/base_tree.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/treap.h"
#include "common/hash.h"
#include "common/template.h"
#include "common/timer.h"
#include "common/vector/hrandom.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

TesterBinarySearchTreeSplitJoin::TesterBinarySearchTreeSplitJoin(
    size_t test_size) {
  vkeys = nvector::HRandom<TKey>(test_size, 0, max_key);
  vkeys.insert(vkeys.end(), vkeys.begin(), vkeys.end());
  std::random_shuffle(vkeys.begin(), vkeys.end());
  vshift = nvector::HRandom<TKey>(test_size * 2, 0, max_key);
  // TODO:
  //   Adjust values in vkeys so they will cancel out after shift.
}

template <class TTree, class TNode>
void TesterBinarySearchTreeSplitJoin::Rotate(TTree& tree, TNode*& root,
                                             TKey value) const {
  TNode *l = nullptr, *r = nullptr;
  tree.SplitByKey(root, value, l, r);
  if (l) l->AddAction(max_key - value);
  if (r) r->AddAction(-value);
  root = tree.Join(r, l);
}

template <class TTree>
size_t TesterBinarySearchTreeSplitJoin::TestBase(
    const std::string& name) const {
  using TNode = typename TTree::TNode;
  Timer t;
  size_t h = 0;
  TTree tree(vkeys.size());  // TODO: half
  TNode* root = nullptr;
  for (unsigned i = 0; i < vkeys.size(); ++i) {
    Rotate(tree, root, vshift[i]);
    if (tree.FindByKey(root, vkeys[i])) {
      root = tree.RemoveAndReleaseByKey(root, vkeys[i]);
    } else {
      root = tree.InsertNewNode(root, {}, vkeys[i]);
    }
    h = HashCombine(h, root ? root->info.sum_keys : 0);
  }
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterBinarySearchTreeSplitJoin::TestAll(bool small_test) const {
  std::unordered_set<size_t> hs;
  if (small_test)
    hs.insert(TestBase<bst::BaseTree<true, TEmpty, TInfo, TAction, TKey>>(
        "Base     "));
  hs.insert(TestBase<bst::SplayTree<true, TEmpty, TInfo, TAction, TKey>>(
      "Splay    "));
  hs.insert(TestBase<bst::Treap<true, false, TEmpty, TInfo, TAction, TKey>>(
      "Treap UPF"));
  hs.insert(TestBase<bst::Treap<true, true, TEmpty, TInfo, TAction, TKey>>(
      "Treap UPT"));
  return hs.size() <= 1;
}

bool TestBinarySearchTreeSplitJoin(bool time_test) {
  return TesterBinarySearchTreeSplitJoin(time_test ? 1000000 : 10000)
      .TestAll(!time_test);
}

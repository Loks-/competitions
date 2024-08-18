#include "tester/tester_binary_search_tree_split_join.h"

#include "common/assert_exception.h"
#include "common/binary_search_tree/avl_tree.h"
#include "common/binary_search_tree/base_tree.h"
#include "common/binary_search_tree/red_black_tree.h"
#include "common/binary_search_tree/scapegoat_tree.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/treap.h"
#include "common/binary_search_tree/wavl_tree.h"
#include "common/hash/combine.h"
#include "common/modular/proxy/proxy.h"
#include "common/template.h"
#include "common/timer.h"
#include "common/vector/hrandom.h"
#include "common/vector/shuffle.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>

TesterBinarySearchTreeSplitJoin::TesterBinarySearchTreeSplitJoin(
    size_t test_size) {
  vkeys = nvector::HRandom<TKey>(test_size, 0, max_key);
  vkeys.insert(vkeys.end(), vkeys.begin(), vkeys.end());
  nvector::Shuffle(vkeys);
  vshift = nvector::HRandom<TKey>(test_size * 2, 0, max_key);
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
  modular::proxy::Proxy<false, true> mp(max_key);
  int64_t shift = 0;
  TTree tree(vkeys.size());
  TNode* root = nullptr;
  size_t h = 0;
  for (unsigned i = 0; i < vkeys.size(); ++i) {
    Rotate(tree, root, vshift[i]);
    shift += vshift[i];
    int64_t key = mp.ApplyS(vkeys[i] - shift);
    if (tree.FindByKey(root, key)) {
      root = tree.RemoveAndReleaseByKey(root, key);
    } else {
      root = tree.InsertNewNode(root, {}, key);
    }
    nhash::DCombineH(h, root ? root->info.sum_keys : 0);
  }
  Assert(!root);
  std::cout << "Test results [" << name << "]: " << h << "\t"
            << t.GetMilliseconds() << std::endl;
  return h;
}

bool TesterBinarySearchTreeSplitJoin::TestAll(bool small_test) const {
  std::unordered_set<size_t> hs;
  if (small_test) {
    hs.insert(TestBase<bst::BaseTree<true, TEmpty, TInfo, TAction, TKey>>(
        "Base  UPT"));
    hs.insert(TestBase<bst::ScapegoatTree<true, TEmpty, TInfo, TAction, TKey>>(
        "Scape UPT"));
  }
  hs.insert(
      TestBase<bst::AVLTree<true, TEmpty, TInfo, TAction, TKey>>("AVL   UPT"));
  hs.insert(
      TestBase<bst::RedBlackTree<TEmpty, TInfo, TAction, TKey>>("RBTree   "));
  hs.insert(TestBase<bst::SplayTree<true, TEmpty, TInfo, TAction, TKey>>(
      "Splay    "));
  hs.insert(TestBase<bst::Treap<true, false, TEmpty, TInfo, TAction, TKey>>(
      "Treap UPF"));
  hs.insert(TestBase<bst::Treap<true, true, TEmpty, TInfo, TAction, TKey>>(
      "Treap UPT"));
  hs.insert(
      TestBase<bst::WAVLTree<true, TEmpty, TInfo, TAction, TKey>>("WAVL  UPT"));
  return hs.size() <= 1;
}

bool TestBinarySearchTreeSplitJoin(bool time_test) {
  return TesterBinarySearchTreeSplitJoin(time_test ? 1000000 : 1000)
      .TestAll(!time_test);
}

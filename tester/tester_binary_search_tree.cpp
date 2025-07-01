#include "tester/tester_binary_search_tree.h"

#include "common/binary_search_tree/avl_tree.h"
#include "common/binary_search_tree/deferred/add_each.h"
#include "common/binary_search_tree/red_black_tree_old.h"
#include "common/binary_search_tree/scapegoat_tree.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/static_tree.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/sum.h"
#include "common/binary_search_tree/treap.h"
#include "common/binary_search_tree/unbalanced_tree.h"
#include "common/binary_search_tree/wavl_tree.h"
#include "common/timer.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

void TesterBinarySearchTree::AddResult(const std::string& task,
                                       unsigned keys_type, size_t h,
                                       size_t time) {
  if (mode == time_test)
    std::cout << "\t\t" << task << "\t" << keys_type << "\t" << time
              << std::endl;
  results.push_back({current_job, current_tree, task, keys_type, h, time});
}

void TesterBinarySearchTree::AddMax() {
  std::unordered_map<std::string, Result> m;
  for (const Result& r : results) {
    std::string skey = r.job + "_" + r.tree + "_" + r.task;
    if (m.find(skey) == m.end()) {
      m[skey] = r;
      m[skey].h = 0;
      m[skey].keys_type = type_end;
    } else {
      m[skey].time = std::max(m[skey].time, r.time);
    }
  }
  for (const auto& p : m) results.push_back(p.second);
}

bool TesterBinarySearchTree::TestHash() const {
  std::unordered_map<std::string, size_t> m;
  for (const Result& r : results) {
    std::string key = r.job + "_" + r.task + "_" + std::to_string(r.keys_type);
    if (m[key] == 0) m[key] = r.h;
    if (m[key] != r.h) {
      std::cout << "ERROR: "
                << "unexpected hash found for:" << std::endl;
      std::cout << "\t" << r.job << "\t" << r.task << "\t" << r.tree << "\t"
                << r.keys_type << "\t" << r.h << std::endl;
      return false;
    }
  }
  return true;
}

void TesterBinarySearchTree::PrintTime() const {
  std::vector<Result> vt = results;
  std::sort(vt.begin(), vt.end(), [](const Result& a, const Result& b) -> bool {
    if (a.job != b.job) return a.job < b.job;
    if (a.task != b.task) return a.task < b.task;
    if (a.keys_type != b.keys_type) return a.keys_type < b.keys_type;
    return a.tree < b.tree;
  });
  std::cout << "Timers:" << std::endl;
  for (const Result& r : vt)
    std::cout << r.job << "\t" << r.task << "\t" << r.keys_type << "\t"
              << r.tree << "\t" << r.time << std::endl;
}

TesterBinarySearchTree::TesterBinarySearchTree(unsigned size, TBSTMode _mode)
    : mode(_mode) {
  keys.resize(type_end);
  keys_sorted.resize(size);
  for (unsigned i = 0; i < size; ++i) keys_sorted[i] = 2 * i + 1;
  keys[sorted] = &keys_sorted;
  keys_reversed = keys_sorted;
  reverse(keys_reversed.begin(), keys_reversed.end());
  keys[reversed] = &keys_reversed;
  keys_shuffled = keys_sorted;
  std::shuffle(keys_shuffled.begin(), keys_shuffled.end(), std::minstd_rand());
  keys[shuffled] = &keys_shuffled;
  keys_shuffled_dups = keys_shuffled;
  for (TKey& key : keys_shuffled_dups) key &= ~2;
  keys[shuffled_dups] = &keys_shuffled_dups;
  keys_one_value.resize(0);
  keys_one_value.resize(size, size | 1);
  keys[one_value] = &keys_one_value;
  keys_sixteen_values = keys_shuffled;
  for (TKey& key : keys_sixteen_values) key &= 31;
  keys[sixteen_values] = &keys_sixteen_values;
}

bool TesterBinarySearchTree::TestAllTrees() {
  using TAggregatorsTuple1 = std::tuple<bst::subtree_data::Size>;
  using TAggregatorsTuple2 =
      std::tuple<bst::subtree_data::Size, bst::subtree_data::Sum<TKey>>;
  using TDeferredTuple2 = std::tuple<bst::deferred::AddEach<TKey>>;
  std::cout << "Testing base trees:" << std::endl;
  current_job = "base";
  if (mode == hash_test) {
    TestAll<bst::UnbalancedTree<true, false, TKey, TAggregatorsTuple1,
                                std::tuple<>, TKey>>("unbalanced_upf");
    TestAll<bst::UnbalancedTree<true, true, TKey, TAggregatorsTuple1,
                                std::tuple<>, TKey>>("unbalanced_upt");
  }
  TestAll<
      bst::AVLTree<true, false, TKey, TAggregatorsTuple1, std::tuple<>, TKey>>(
      "avltree_upf");
  TestAll<
      bst::AVLTree<true, true, TKey, TAggregatorsTuple1, std::tuple<>, TKey>>(
      "avltree_upt");
  TestAll<bst::StaticTree<true, false, TKey, TAggregatorsTuple1, std::tuple<>,
                          TKey>>("static_upf");
  TestAll<bst::StaticTree<true, true, TKey, TAggregatorsTuple1, std::tuple<>,
                          TKey>>("static_upt");
  TestAll<bst::RedBlackTreeOld<TKey, TAggregatorsTuple1, std::tuple<>, TKey>>(
      "rbtree");
  TestAll<bst::ScapegoatTree<true, false, TKey, TAggregatorsTuple1,
                             std::tuple<>, TKey>>("scape_upf");
  TestAll<bst::ScapegoatTree<true, true, TKey, TAggregatorsTuple1, std::tuple<>,
                             TKey>>("scape_upt");
  TestAll<bst::SplayTree<true, TKey, TAggregatorsTuple1, std::tuple<>, TKey>>(
      "splay_upt");
  TestAll<
      bst::Treap<true, false, TKey, TAggregatorsTuple1, std::tuple<>, TKey>>(
      "treap_upf");
  TestAll<bst::Treap<true, true, TKey, TAggregatorsTuple1, std::tuple<>, TKey>>(
      "treap_upt");
  TestAll<
      bst::WAVLTree<true, false, TKey, TAggregatorsTuple1, std::tuple<>, TKey>>(
      "wavltree_upf");
  TestAll<
      bst::WAVLTree<true, true, TKey, TAggregatorsTuple1, std::tuple<>, TKey>>(
      "wavltree_upt");

  std::cout << "Testing full trees:" << std::endl;
  current_job = "full";
  if (mode == hash_test) {
    TestAll<bst::UnbalancedTree<true, false, TKey, TAggregatorsTuple2,
                                TDeferredTuple2, TKey>>("unbalanced_upf");
    TestAll<bst::UnbalancedTree<true, true, TKey, TAggregatorsTuple2,
                                TDeferredTuple2, TKey>>("unbalanced_upt");
  }
  TestAll<bst::AVLTree<true, false, TKey, TAggregatorsTuple2, TDeferredTuple2,
                       TKey>>("avltree_upf");
  TestAll<bst::AVLTree<true, true, TKey, TAggregatorsTuple2, TDeferredTuple2,
                       TKey>>("avltree_upt");
  TestAll<
      bst::RedBlackTreeOld<TKey, TAggregatorsTuple2, TDeferredTuple2, TKey>>(
      "rbtree");
  TestAll<bst::ScapegoatTree<true, false, TKey, TAggregatorsTuple2,
                             TDeferredTuple2, TKey>>("scape_upf");
  TestAll<bst::ScapegoatTree<true, true, TKey, TAggregatorsTuple2,
                             TDeferredTuple2, TKey>>("scape_upt");
  TestAll<
      bst::SplayTree<true, TKey, TAggregatorsTuple2, TDeferredTuple2, TKey>>(
      "splay_upt");
  TestAll<
      bst::Treap<true, false, TKey, TAggregatorsTuple2, TDeferredTuple2, TKey>>(
      "treap_upf");
  TestAll<
      bst::Treap<true, true, TKey, TAggregatorsTuple2, TDeferredTuple2, TKey>>(
      "treap_upt");
  TestAll<bst::WAVLTree<true, false, TKey, TAggregatorsTuple2, TDeferredTuple2,
                        TKey>>("wavltree_upf");
  TestAll<bst::WAVLTree<true, true, TKey, TAggregatorsTuple2, TDeferredTuple2,
                        TKey>>("wavltree_upt");

  bool output = TestHash();
  if ((mode == time_test) && output) {
    AddMax();
    PrintTime();
  }
  return output;
}

bool TestBinarySearchTree(bool time_test) {
  Timer t;
  TesterBinarySearchTree tbst(time_test ? 1000000 : 1000,
                              time_test ? TesterBinarySearchTree::time_test
                                        : TesterBinarySearchTree::hash_test);
  bool result = tbst.TestAllTrees();
  if (result && time_test)
    std::cout << "Total time = " << t.get_milliseconds() << std::endl;
  return result;
}

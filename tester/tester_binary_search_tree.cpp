#include "tester/tester_binary_search_tree.h"

#include "common/binary_search_tree/avl_tree.h"
#include "common/binary_search_tree/base_tree.h"
#include "common/binary_search_tree/perfect_tree.h"
#include "common/binary_search_tree/red_black_tree.h"
#include "common/binary_search_tree/scapegoat_tree.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/treap.h"
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
  std::cout << "Testing base trees:" << std::endl;
  current_job = "base";
  if (mode == hash_test) {
    TestAll<
        bst::BaseTree<false, TKey, bst::info::Size, bst::action::None, TKey>>(
        "base_upf");
    TestAll<
        bst::BaseTree<true, TKey, bst::info::Size, bst::action::None, TKey>>(
        "base_upt");
  }
  TestAll<bst::AVLTree<false, TKey, bst::info::Size, bst::action::None, TKey>>(
      "avltree_upf");
  TestAll<bst::AVLTree<true, TKey, bst::info::Size, bst::action::None, TKey>>(
      "avltree_upt");
  TestAll<
      bst::PerfectTree<false, TKey, bst::info::Size, bst::action::None, TKey>>(
      "perfect_upf");
  TestAll<
      bst::PerfectTree<true, TKey, bst::info::Size, bst::action::None, TKey>>(
      "perfect_upt");
  TestAll<bst::RedBlackTree<TKey, bst::info::Size, bst::action::None, TKey>>(
      "rbtree");
  TestAll<bst::ScapegoatTree<false, TKey, bst::info::Size, bst::action::None,
                             TKey>>("scape_upf");
  TestAll<
      bst::ScapegoatTree<true, TKey, bst::info::Size, bst::action::None, TKey>>(
      "scape_upt");
  TestAll<bst::SplayTree<true, TKey, bst::info::Size, bst::action::None, TKey>>(
      "splay_upt");
  TestAll<
      bst::Treap<true, false, TKey, bst::info::Size, bst::action::None, TKey>>(
      "treap_upf");
  TestAll<
      bst::Treap<true, true, TKey, bst::info::Size, bst::action::None, TKey>>(
      "treap_upt");
  TestAll<bst::WAVLTree<false, TKey, bst::info::Size, bst::action::None, TKey>>(
      "wavltree_upf");
  TestAll<bst::WAVLTree<true, TKey, bst::info::Size, bst::action::None, TKey>>(
      "wavltree_upt");

  std::cout << "Testing full trees:" << std::endl;
  current_job = "full";
  if (mode == hash_test) {
    TestAll<bst::BaseTree<false, TKey, bst::info::Sum<TKey, bst::info::Size>,
                          bst::action::AddEachSum<TKey>, TKey>>("base_upf");
    TestAll<bst::BaseTree<true, TKey, bst::info::Sum<TKey, bst::info::Size>,
                          bst::action::AddEachSum<TKey>, TKey>>("base_upt");
  }
  TestAll<bst::AVLTree<false, TKey, bst::info::Sum<TKey, bst::info::Size>,
                       bst::action::AddEachSum<TKey>, TKey>>("avltree_upf");
  TestAll<bst::AVLTree<true, TKey, bst::info::Sum<TKey, bst::info::Size>,
                       bst::action::AddEachSum<TKey>, TKey>>("avltree_upt");
  TestAll<bst::RedBlackTree<TKey, bst::info::Sum<TKey, bst::info::Size>,
                            bst::action::AddEachSum<TKey>, TKey>>("rbtree");
  TestAll<bst::ScapegoatTree<false, TKey, bst::info::Sum<TKey, bst::info::Size>,
                             bst::action::AddEachSum<TKey>, TKey>>("scape_upf");
  TestAll<bst::ScapegoatTree<true, TKey, bst::info::Sum<TKey, bst::info::Size>,
                             bst::action::AddEachSum<TKey>, TKey>>("scape_upt");
  TestAll<bst::SplayTree<true, TKey, bst::info::Sum<TKey, bst::info::Size>,
                         bst::action::AddEachSum<TKey>, TKey>>("splay_upt");
  TestAll<bst::Treap<true, false, TKey, bst::info::Sum<TKey, bst::info::Size>,
                     bst::action::AddEachSum<TKey>, TKey>>("treap_upf");
  TestAll<bst::Treap<true, true, TKey, bst::info::Sum<TKey, bst::info::Size>,
                     bst::action::AddEachSum<TKey>, TKey>>("treap_upt");
  TestAll<bst::WAVLTree<false, TKey, bst::info::Sum<TKey, bst::info::Size>,
                        bst::action::AddEachSum<TKey>, TKey>>("wavltree_upf");
  TestAll<bst::WAVLTree<true, TKey, bst::info::Sum<TKey, bst::info::Size>,
                        bst::action::AddEachSum<TKey>, TKey>>("wavltree_upt");

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
    std::cout << "Total time = " << t.GetMilliseconds() << std::endl;
  return result;
}

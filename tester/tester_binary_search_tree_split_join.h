#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/add_each_key__sum_keys.h"
#include "common/binary_search_tree/base/subtree_data.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/sum_keys.h"

#include <string>
#include <vector>

class TesterBinarySearchTreeSplitJoin {
 public:
  using TKey = int64_t;
  using TSize = bst::subtree_data::Size;
  using TSum = bst::subtree_data::SumKeys<TKey>;
  using TAggregators = std::tuple<TSize, TSum>;
  using TInfo = bst::base::SubtreeData<TAggregators>;
  using TAction =
      bst::action::AddEachKeySumKeys<bst::subtree_data::SumKeys<TKey>, TKey>;

 protected:
  const TKey max_key = (1ll << 30);
  std::vector<TKey> vkeys;
  std::vector<TKey> vshift;

 public:
  TesterBinarySearchTreeSplitJoin(size_t test_size);

 protected:
  template <class TTree, class TNode>
  void Rotate(TTree& tree, TNode*& root, TKey value) const;

  template <class TTree>
  size_t TestBase(const std::string& name) const;

 public:
  bool TestAll(bool small_test) const;
};

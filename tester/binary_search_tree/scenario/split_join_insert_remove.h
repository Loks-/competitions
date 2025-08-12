#pragma once

#include "tester/binary_search_tree/data.h"
#include "tester/binary_search_tree/scenario/base.h"
#include "tester/binary_search_tree/scenario/result.h"
#include "tester/hash_combine.h"

#include "common/assert_exception.h"
#include "common/base.h"
#include "common/binary_search_tree/auto/find.h"
#include "common/binary_search_tree/deferred/add_each_key.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/sum_keys.h"
#include "common/template.h"
#include "common/timer.h"

#include <chrono>
#include <string>
#include <tuple>
#include <vector>

namespace tester {
namespace bst {
namespace scenario {

template <DataType data_type>
class SplitJoinInsertRemove : public Base<SplitJoinInsertRemove<data_type>> {
 public:
  static constexpr bool requires_key = true;
  static constexpr bool requires_insert = true;
  static constexpr bool requires_remove = true;
  static constexpr bool requires_split = true;
  static constexpr bool requires_join = true;

  using Data = int64_t;
  using Key = int64_t;
  using Aggregator = ::bst::subtree_data::SumKeys<Key>;
  using AggregatorsTuple = std::tuple<::bst::subtree_data::Size, Aggregator>;
  using DeferredTuple = std::tuple<::bst::deferred::AddEachKey<Key>>;

  static constexpr std::string id() {
    return std::string("split_join_insert_remove @ ") + get_name(data_type);
  }

  template <bool extra_checks, template <typename, typename, typename,
                                         typename> class TImplementation>
  static std::pair<size_t, std::chrono::nanoseconds> run_impl(
      size_t size, std::string& implementation_id) {
    using Implementation =
        TImplementation<Data, Key, AggregatorsTuple, DeferredTuple>;
    using Tree = typename Implementation::TreeType;
    using Node = typename Tree::NodeType;

    implementation_id = Implementation::id();
    const auto& values = get_data_int64(data_type, size);
    Tree tree(size);

    Timer timer;
    timer.start();
    size_t hash = 0;
    Node* root = nullptr;
    Key shift = 0;
    const Key max_key = Key(1) << 30;

    for (auto value : values) {
      Key key = value + shift;
      key %= max_key;
      if (key < 0) key += max_key;
      Node* node = ::bst::auto_::find<Tree>(root, key);
      if (node) {
        if constexpr (extra_checks)
          assert_exception(node->data == value, "Node value mismatch");
        root = tree.remove_and_release(root, key);
      } else {
        root = tree.insert_new(root, value, key);
      }

      Key threshold = value % max_key;
      if (threshold < 0) threshold += max_key;
      Node *l = nullptr, *r = nullptr;
      Tree::split(root, threshold, l, r);
      ::bst::deferred::add_to_each_key(l, max_key - threshold);
      ::bst::deferred::add_to_each_key(r, Key(-threshold));
      root = Tree::join(r, l);
      shift = (shift - threshold) % max_key;

      hash_combine(hash, Aggregator::get(root));
    }

    if constexpr (extra_checks) assert_exception(!root, "Root is not null");
    tree.release_tree(root);
    if constexpr (extra_checks)
      assert_exception(tree.used() == 0, "Memory usage is not 0");
    hash_combine(hash, tree.used());

    timer.stop();
    return {hash, timer.get_duration()};
  }
};

}  // namespace scenario
}  // namespace bst
}  // namespace tester

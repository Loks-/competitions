#pragma once

#include "tester/binary_search_tree/data.h"
#include "tester/binary_search_tree/scenario/base.h"
#include "tester/binary_search_tree/scenario/result.h"
#include "tester/binary_search_tree/utils/verify_parent_links.h"
#include "tester/hash_combine.h"

#include "common/assert_exception.h"
#include "common/base.h"
#include "common/binary_search_tree/deferred/add_each.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/binary_search_tree/subtree_data/sum.h"
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
class SplitJoinAddEach : public Base<SplitJoinAddEach<data_type>> {
 public:
  static constexpr bool requires_key = false;
  static constexpr bool requires_split = true;
  static constexpr bool requires_join = true;

  using Data = int64_t;
  using Key = int64_t;
  using Aggregator = ::bst::subtree_data::Sum<Data>;
  using AggregatorsTuple = std::tuple<::bst::subtree_data::Size, Aggregator>;
  using DeferredTuple = std::tuple<::bst::deferred::AddEach<Data>>;

  static constexpr std::string id() {
    return std::string("split_join_add_each @ ") + get_name(data_type);
  }

  template <bool extra_checks, template <typename, typename, typename,
                                         typename> class TImplementation>
  static std::pair<size_t, std::chrono::nanoseconds> run_impl(
      size_t size, std::string& implementation_id) {
    using Implementation =
        TImplementation<Data, Key, AggregatorsTuple, DeferredTuple>;
    using Tree = typename Implementation::TreeType;

    implementation_id = Implementation::id();
    const auto& lsizes = get_data_int64(data_type, size);
    Tree tree(size);

    Timer timer;
    timer.start();
    size_t hash = 0;

    auto root = tree.build(std::vector<Data>(size, Data(0)));
    if constexpr (extra_checks) {
      assert_exception(root, "Root is null");
      assert_exception(::bst::subtree_data::size(root) == size,
                       "Root size is not the same as the size of the tree");
      if constexpr (Tree::has_parent)
        assert_exception(verify_parent_links(root),
                         "Parent links are not valid");
    }
    hash_combine(hash, ::bst::subtree_data::size(root));

    for (auto lsize_raw : lsizes) {
      const size_t lsize = static_cast<size_t>(lsize_raw % (size + 1));
      typename Tree::NodeType *l = nullptr, *r = nullptr;
      Tree::split_at(root, lsize, l, r);
      ::bst::deferred::add_to_each(l, Data(1));
      root = Tree::join(r, l);
      if constexpr (extra_checks) {
        assert_exception(::bst::subtree_data::size(root) == size,
                         "Root size is not the same as the size of the tree");
      }
      hash_combine(hash, Aggregator::get(root));
    }

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

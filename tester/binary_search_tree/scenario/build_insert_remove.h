#pragma once

#include "tester/binary_search_tree/data.h"
#include "tester/binary_search_tree/scenario/base.h"
#include "tester/binary_search_tree/scenario/result.h"
#include "tester/binary_search_tree/utils/verify_parent_links.h"
#include "tester/hash_combine.h"

#include "common/assert_exception.h"
#include "common/base.h"
#include "common/binary_search_tree/auto/at.h"
#include "common/binary_search_tree/auto/find.h"
#include "common/binary_search_tree/subtree_data/size.h"
#include "common/template.h"
#include "common/timer.h"

#include <chrono>
#include <string>
#include <tuple>

namespace tester {
namespace bst {
namespace scenario {

template <DataType data_type>
class BuildInsertRemove : public Base<BuildInsertRemove<data_type>> {
 public:
  static constexpr bool requires_insert = true;
  static constexpr bool requires_remove = true;

  using Data = MetaEmpty;
  using Key = int64_t;
  using AggregatorsTuple = std::tuple<::bst::subtree_data::Size>;
  using DeferredTuple = std::tuple<>;

  static constexpr std::string id() {
    return std::string("build_insert_remove @ ") + get_name(data_type);
  }

  template <bool extra_checks, template <typename, typename, typename,
                                         typename> class TImplementation>
  static std::pair<size_t, std::chrono::nanoseconds> run_impl(
      size_t size, std::string& implementation_id) {
    using Implementation =
        TImplementation<Data, Key, AggregatorsTuple, DeferredTuple>;
    using Tree = typename Implementation::TreeType;

    implementation_id = Implementation::id();
    const auto& keys = get_data_int64(data_type, size);
    Tree tree(size);

    Timer timer;
    timer.start();
    size_t hash = 0;
    typename Tree::NodeType* root = nullptr;

    // Build initial tree of size/4
    const size_t initial_size = size / 4;
    std::vector<Data> initial_data(initial_size);
    std::vector<Key> initial_keys(keys.begin(), keys.begin() + initial_size);
    root = tree.build(initial_data, initial_keys);
    if constexpr (extra_checks) {
      assert_exception(root, "Root is null");
      assert_exception(::bst::subtree_data::size(root) == initial_size,
                       "Root size is not the same as the size of the tree");
    }
    hash_combine(hash, ::bst::subtree_data::size(root));

    // Start insert/remove operations from size/4
    for (unsigned i = initial_size, j = 0; j < size;) {
      if ((i < size) && (i - j < size / 2)) {
        // insert
        root = tree.insert_new(root, Data(), keys[i++]);
        if constexpr (extra_checks) {
          assert_exception(root, "Root is null");
          assert_exception(::bst::subtree_data::size(root) == i - j,
                           "Root size is not the same as the size of the tree");
          if constexpr (Tree::has_parent) {
            if ((i == size / 2) || (i == size)) {
              assert_exception(verify_parent_links(root),
                               "Parent links are not valid");
            }
          }
        }
        hash_combine(hash, ::bst::subtree_data::size(root));

        const auto k = (i + j) / 2;
        auto node = ::bst::auto_::find<Tree>(root, keys[k]);
        if constexpr (extra_checks) {
          assert_exception(node && (node->key == keys[k]),
                           "Node is null or key is not the same as the key");
        }
        hash_combine(hash, node ? node->key : 0);
      } else {
        // remove
        root = tree.remove_and_release(root, keys[j++]);
        if constexpr (extra_checks) {
          assert_exception(::bst::subtree_data::size(root) == i - j,
                           "Root size is not the same as the size of the tree");
          if constexpr (Tree::has_parent) {
            if (j == 3 * size / 4) {
              assert_exception(verify_parent_links(root),
                               "Parent links are not valid");
            }
          }
        }
        hash_combine(hash, ::bst::subtree_data::size(root));

        if (j < i) {
          const auto k = (i + j) / 2;
          auto node = ::bst::auto_::at<Tree>(root, k - j);
          hash_combine(hash, node ? node->key : 0);
        }
      }
    }

    timer.stop();
    return {hash, timer.get_duration()};
  }
};

}  // namespace scenario
}  // namespace bst
}  // namespace tester

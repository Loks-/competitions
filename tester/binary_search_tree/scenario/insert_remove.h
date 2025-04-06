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
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace tester {
namespace bst {
namespace scenario {

template <DataType data_type>
class InsertRemove : public Base<InsertRemove<data_type>> {
 public:
  static constexpr bool requires_insert = true;
  static constexpr bool requires_remove = true;

  using Data = MetaEmpty;
  using Key = int64_t;
  using AggregatorsTuple = std::tuple<::bst::subtree_data::Size>;
  using DeferredTuple = std::tuple<>;

  static constexpr std::string id() {
    return std::string("insert_remove @ ") + get_name(data_type);
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

    for (unsigned i = 0, j = 0; j < size;) {
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

    // Check memory usage
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

#pragma once

#include "common/base.h"

namespace bst {
namespace utils {

/**
 * @brief Adds a deferred operation to all nodes in a segment defined by index
 * range.
 *
 * This function splits the tree into three parts based on the given index
 * range:
 * - Left part: nodes with indices less than 'begin'
 * - Middle part: nodes with indices in the range [begin, end)
 * - Right part: nodes with indices greater than or equal to 'end'
 *
 * The deferred operation is applied to all nodes in the middle segment, and
 * then the three parts are rejoined to form the updated tree.
 *
 * Trivial cases are handled gracefully:
 * - If root is nullptr, returns nullptr
 * - If begin >= end, returns root unchanged
 *
 * @tparam TDeferred The deferred operation type that provides add method.
 * @tparam TTree The tree type that provides split_at, join, and TNode types.
 * @tparam TNode The node type.
 * @tparam TDeferredValue The type of the deferred operation value to be added.
 * @param root The root of the tree to modify.
 * @param begin The beginning of the index range (inclusive).
 * @param end The end of the index range (exclusive).
 * @param deferred_value The deferred operation value to add to nodes in the
 * segment.
 * @return Pointer to the root of the modified tree.
 */
template <class TDeferred, class TTree, class TNode, class TDeferredValue>
[[nodiscard]] constexpr TNode* add_deferred_to_segment_by_index(
    TNode* root, size_t begin, size_t end,
    const TDeferredValue& deferred_value) {
  // Handle trivial cases
  if (!root || !(begin < end)) return root;

  TNode *left_part, *middle_part, *right_part;

  // Split the tree at 'end' to get middle and right parts
  TTree::split_at(root, end, middle_part, right_part);

  // Split the middle part at 'begin' to get left and middle parts
  TTree::split_at(middle_part, begin, left_part, middle_part);

  // Apply the deferred operation to all nodes in the middle segment
  if (middle_part) TDeferred::add(middle_part, deferred_value);

  // Rejoin the three parts: (left_part + middle_part) + right_part
  return TTree::join(TTree::join(left_part, middle_part), right_part);
}

}  // namespace utils
}  // namespace bst

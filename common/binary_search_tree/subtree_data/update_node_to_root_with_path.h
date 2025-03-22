#pragma once

#include "common/base.h"
#include "common/template.h"

#include <vector>

namespace bst {
namespace subtree_data {

/**
 * @brief No-op implementation for nodes with no subtree data.
 *
 * @tparam Node The node type.
 * @param node_to_root_path The path from a node to the root.
 * @param start_from_index The index in the path to start updating from.
 */
template <typename Node>
constexpr void update_node_to_root_with_path(
    const std::vector<Node*>& node_to_root_path, size_t start_from_index,
    MetaFalse) {}

/**
 * @brief Updates subtree data for nodes along a path from a node to the root.
 *
 * @tparam Node The node type.
 * @param node_to_root_path The path from a node to the root.
 * @param start_from_index The index in the path to start updating from.
 */
template <typename Node>
inline void update_node_to_root_with_path(
    const std::vector<Node*>& node_to_root_path, size_t start_from_index,
    MetaTrue) {
  for (auto it = node_to_root_path.begin() + start_from_index;
       it != node_to_root_path.end(); ++it)
    (*it)->UpdateInfo();
}

/**
 * @brief Updates subtree data for nodes along a path from a node to the root.
 *
 * This function is only active for nodes that have non-empty subtree data.
 *
 * @tparam Node The node type.
 * @param node_to_root_path The path from a node to the root.
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename Node>
inline void update_node_to_root_with_path(
    const std::vector<Node*>& node_to_root_path, size_t start_from_index = 0) {
  update_node_to_root_with_path(node_to_root_path, start_from_index,
                                MetaBool<!Node::TInfo::is_none>());
}

/**
 * @brief Updates subtree data for nodes along a path when node data has been
 * modified.
 *
 * This function is only active for nodes that use node data in their subtree
 * data.
 *
 * @tparam Node The node type.
 * @param node_to_root_path The path from a node to the root.
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename Node>
inline void update_node_to_root_with_path_data_updated(
    const std::vector<Node*>& node_to_root_path, size_t start_from_index = 0) {
  update_node_to_root_with_path(node_to_root_path, start_from_index,
                                MetaBool<Node::TInfo::use_data>());
}

/**
 * @brief Updates subtree data for nodes along a path when node keys have been
 * modified.
 *
 * This function is only active for nodes that use node keys in their subtree
 * data.
 *
 * @tparam Node The node type.
 * @param node_to_root_path The path from a node to the root.
 * @param start_from_index The index in the path to start updating from
 * (default: 0).
 */
template <typename Node>
inline void update_node_to_root_with_path_keys_updated(
    const std::vector<Node*>& node_to_root_path, size_t start_from_index = 0) {
  update_node_to_root_with_path(node_to_root_path, start_from_index,
                                MetaBool<Node::TInfo::use_keys>());
}

}  // namespace subtree_data
}  // namespace bst

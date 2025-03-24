#pragma once

#include "common/template.h"

namespace bst {
namespace subtree_data {
namespace hidden {

/**
 * @brief No-op implementation for nodes with empty subtree data.
 *
 * @tparam Node The node type.
 */
template <typename Node>
constexpr void update_node_to_root_i(Node*, MetaFalse) {}

/**
 * @brief Updates subtree data from a node up to the root.
 *
 * @tparam Node The node type.
 */
template <typename Node>
inline void update_node_to_root_i(Node* node, MetaTrue) {
  static_assert(Node::use_parent, "use_parent should be true");
  for (; node; node = node->p) node->UpdateInfo();
}

}  // namespace hidden

/**
 * @brief Updates subtree data from a node up to the root.
 *
 * This function is only active for nodes that have non-empty subtree data.
 *
 * @tparam Node The node type.
 * @param node The node to start updating from.
 */
template <typename Node>
inline void update_node_to_root(Node* node) {
  hidden::update_node_to_root_i(node, MetaBool<!Node::TInfo::empty>{});
}

/**
 * @brief Updates subtree data from a node up to the root when node data has
 * been modified.
 *
 * This function is only active for nodes that use node data in their subtree
 * data.
 *
 * @tparam Node The node type.
 * @param node The node to start updating from.
 */
template <typename Node>
inline void update_node_to_root_data_updated(Node* node) {
  hidden::update_node_to_root_i(node, MetaBool<Node::TInfo::use_data>{});
}

/**
 * @brief Updates subtree data from a node up to the root when node keys have
 * been modified.
 *
 * This function is only active for nodes that use node keys in their subtree
 * data.
 *
 * @tparam Node The node type.
 * @param node The node to start updating from.
 */
template <typename Node>
inline void update_node_to_root_keys_updated(Node* node) {
  hidden::update_node_to_root_i(node, MetaBool<Node::TInfo::use_keys>{});
}

}  // namespace subtree_data
}  // namespace bst

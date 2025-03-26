#pragma once

#include "common/template.h"

namespace ds {
namespace st {
namespace info {
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

template <typename Node>
inline void UpdateNodeToRoot(Node* node) {
  hidden::update_node_to_root_i(node, MetaBool<!Node::TInfo::empty>{});
}

}  // namespace info
}  // namespace st
}  // namespace ds

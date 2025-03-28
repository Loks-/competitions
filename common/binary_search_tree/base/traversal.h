#pragma once

#include "common/base.h"

#include <queue>
#include <vector>

namespace bst {
namespace base {
namespace hidden {
template <class TNode, class TData>
inline void TraversePreorder(TNode* node, std::vector<TData>& output) {
  if (!node) return;
  node->apply_deferred();
  output.push_back(node->data);
  TraversePreorder(node->left, output);
  TraversePreorder(node->right, output);
}

template <class TNode, class TData>
inline void TraverseInorder(TNode* node, std::vector<TData>& output) {
  if (!node) return;
  node->apply_deferred();
  TraverseInorder(node->left, output);
  output.push_back(node->data);
  TraverseInorder(node->right, output);
}

template <class TNode, class TData>
inline void TraversePostorder(TNode* node, std::vector<TData>& output) {
  if (!node) return;
  node->apply_deferred();
  TraversePostorder(node->left, output);
  TraversePostorder(node->right, output);
  output.push_back(node->data);
}

template <class TNode, class TData>
inline void TraverseLevelorder(TNode* node, std::vector<TData>& output) {
  if (!node) return;
  thread_local std::queue<TNode*> q;
  for (q.push(node); !q.empty(); q.pop()) {
    node = q.front();
    node->apply_deferred();
    output.push_back(node->data);
    if (node->left) q.push(node->left);
    if (node->right) q.push(node->right);
  }
}
}  // namespace hidden

enum class ETraversalOrder { Preorder, Inorder, Postorder, Levelorder };

template <class TNode, class TData>
inline std::vector<TData> Traverse(TNode* root, ETraversalOrder order) {
  std::vector<TData> output;
  switch (order) {
    case ETraversalOrder::Preorder:
      hidden::TraversePreorder(root, output);
      break;
    case ETraversalOrder::Inorder:
      hidden::TraverseInorder(root, output);
      break;
    case ETraversalOrder::Postorder:
      hidden::TraversePostorder(root, output);
      break;
    case ETraversalOrder::Levelorder:
      hidden::TraverseLevelorder(root, output);
      break;
    default:
      assert(false);
  }
  return output;
}
}  // namespace base
}  // namespace bst

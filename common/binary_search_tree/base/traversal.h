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
  node->ApplyAction();
  output.push_back(node->data);
  TraversePreorder(node->l, output);
  TraversePreorder(node->r, output);
}

template <class TNode, class TData>
inline void TraverseInorder(TNode* node, std::vector<TData>& output) {
  if (!node) return;
  node->ApplyAction();
  TraverseInorder(node->l, output);
  output.push_back(node->data);
  TraverseInorder(node->r, output);
}

template <class TNode, class TData>
inline void TraversePostorder(TNode* node, std::vector<TData>& output) {
  if (!node) return;
  node->ApplyAction();
  TraversePostorder(node->l, output);
  TraversePostorder(node->r, output);
  output.push_back(node->data);
}

template <class TNode, class TData>
inline void TraverseLevelorder(TNode* node, std::vector<TData>& output) {
  if (!node) return;
  thread_local std::queue<TNode*> q;
  for (q.push(node); !q.empty(); q.pop()) {
    node = q.front();
    node->ApplyAction();
    output.push_back(node->data);
    if (node->l) q.push(node->l);
    if (node->r) q.push(node->r);
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

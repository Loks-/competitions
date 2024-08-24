#pragma once

namespace bst {
namespace info {
namespace segment {
template <class TNode>
constexpr typename TNode::TInfo GetByKey(const TNode* root,
                                         const typename TNode::TKey& end) {
  typename TNode::TInfo output;
  for (; root;) {
    if (root->key < end) {
      output.AddS(root->l);
      output.AddN(root);
      root = root->r;
    } else {
      root = root->l;
    }
  }
  return output;
}

template <class TNode>
constexpr typename TNode::TInfo GetByKey(const TNode* root,
                                         const typename TNode::TKey& begin,
                                         const typename TNode::TKey& end) {
  for (; root;) {
    if (root->key < begin)
      root = root->r;
    else if (root->key >= end)
      root = root->l;
    else
      break;
  }
  typename TNode::TInfo output;
  if (!root) return output;
  output.SetN(root);
  for (const TNode* node = root->l; node;) {
    if (node->key < begin) {
      node = node->r;
    } else {
      output.AddN(node);
      output.AddS(node->r);
      node = node->l;
    }
  }
  for (const TNode* node = root->r; node;) {
    if (node->key < end) {
      output.AddS(node->l);
      output.AddN(node);
      node = node->r;
    } else {
      node = node->l;
    }
  }
  return output;
}
}  // namespace segment
}  // namespace info
}  // namespace bst

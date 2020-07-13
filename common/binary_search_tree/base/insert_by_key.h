#pragma once

#include "common/binary_search_tree/info/helpers/update_node_to_root.h"
#include "common/template.h"

#include <stack>

namespace bst {
template <class TNode>
inline void InsertByKeyUseInsert(TNode* root, TNode* node) {
  for (;;) {
    root->ApplyAction();
    root->info.Insert(node);
    if (root->key < node->key) {
      if (root->r) {
        root = root->r;
      } else {
        root->SetR(node);
        break;
      }
    } else {
      if (root->l) {
        root = root->l;
      } else {
        root->SetL(node);
        break;
      }
    }
  }
}

template <class TNode>
inline void InsertByKeyUseParent(TNode* root, TNode* node) {
  for (;;) {
    root->ApplyAction();
    if (root->key < node->key) {
      if (root->r) {
        root = root->r;
      } else {
        root->SetR(node);
        break;
      }
    } else {
      if (root->l) {
        root = root->l;
      } else {
        root->SetL(node);
        break;
      }
    }
  }
  UpdateNodeToRoot(root);
}

template <class TNode>
inline void InsertByKeyDefault(TNode* root, TNode* node) {
  thread_local std::stack<TNode*> s;
  for (;;) {
    s.push(root);
    root->ApplyAction();
    if (root->key < node->key) {
      if (root->r) {
        root = root->r;
      } else {
        root->SetR(node);
        break;
      }
    } else {
      if (root->l) {
        root = root->l;
      } else {
        root->SetL(node);
        break;
      }
    }
  }
  for (; !s.empty(); s.pop()) s.top()->UpdateInfo();
}

namespace hidden {
template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, TFakeFalse, TFakeFalse) {
  InsertByKeyDefault<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, TFakeFalse, TFakeTrue) {
  InsertByKeyUseParent<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, TFakeTrue, TFakeFalse) {
  InsertByKeyUseInsert<TNode>(root, node);
}

template <class TNode>
inline void InsertByKeyI(TNode* root, TNode* node, TFakeTrue, TFakeTrue) {
  InsertByKeyUseInsert<TNode>(root, node);
}
}  // namespace hidden

template <class TNode>
inline void InsertByKey(TNode* root, TNode* node) {
  static_assert(TNode::use_key, "use_key should be true");
  if (!root || !node) return;
  hidden::InsertByKeyI<TNode>(root, node, TFakeBool<TNode::support_insert>(),
                              TFakeBool<TNode::use_parent>());
}
}  // namespace bst

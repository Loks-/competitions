#pragma once

#include "none.h"

class BSTInfoSize : public BSTInfoNone {
 public:
  using TSelf = BSTInfoSize;
  static const bool is_none = false;
  static const bool has_size = true;

  unsigned size = 0;

  template <class TNode>
  void Update(TNode* node) {
    node->ApplyAction();
    size = 1 + (node->l ? node->l->info.size : 0) +
           (node->r ? node->r->info.size : 0);
  }
};

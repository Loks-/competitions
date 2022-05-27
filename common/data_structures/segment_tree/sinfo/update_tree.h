#pragma once

#include "common/base.h"

namespace ds {
namespace st {
namespace sinfo {
template <class TNode>
inline void SUpdateTree(TNode* node) {
  assert(node);
  if (node->IsLeaf()) {
    node->sinfo.UpdateData(node->GetData());
  } else {
    SUpdateTree(node->l);
    SUpdateTree(node->r);
    node->sinfo.UpdateLR(node->l->sinfo, node->r->sinfo);
  }
}
}  // namespace sinfo
}  // namespace st
}  // namespace ds

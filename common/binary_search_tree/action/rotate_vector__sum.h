#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/subtree_data/sum.h"

namespace bst {
namespace action {
// Support only VectorStaticSize from LinearAlgebra.
template <class TVector>
class RotateVectorSum : public None {
 protected:
  static void Rotate(TVector& v, size_t r) {
    if (r == 0) return;
    thread_local TVector vt;
    const size_t size = v.Size();
    for (size_t i = 0; i < size; ++i) vt((i + r) % size) = v(i);
    v.swap(vt);
  }

 public:
  using TBase = None;
  using TSelf = RotateVectorSum;
  using TSum = bst::subtree_data::Sum<TVector>;

  static constexpr bool is_none = false;
  static constexpr bool modify_data = true;

 public:
  size_t r = 0;

 public:
  constexpr bool IsEmpty() const { return r == 0; }

  constexpr void Clear() { r = 0; };

  template <class TNode>
  void Add(TNode* node, size_t _r) {
    Rotate(TSum::get_ref(node), _r);
    r = (r + _r) % TSum::get(node).Size();
  }

  template <class TNode>
  void Apply(TNode* node) {
    if (IsEmpty()) return;
    Rotate(node->data, r);
    if (node->l) node->l->AddAction(r);
    if (node->r) node->r->AddAction(r);
    r = 0;
  }
};
}  // namespace action
}  // namespace bst

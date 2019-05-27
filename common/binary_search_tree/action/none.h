#pragma once

namespace bst {
namespace action {
class None {
 public:
  using TSelf = None;
  static const bool is_none = true;

  bool IsEmpty() const { return true; }
  void Clear(){};
  template <class TNode, class TActionValue>
  void Add(TNode* node, const TActionValue& value) {}
  template <class TNode>
  void Apply(const TNode* node) {}
};
}  // namespace action
}  // namespace bst

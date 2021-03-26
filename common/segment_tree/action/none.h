#pragma once

namespace st {
namespace action {
class None {
 public:
  using TSelf = None;
  static const bool is_none = true;

  bool IsEmpty() const { return true; }
  void Clear(){};

  template <class TNode, class TActionValue>
  void Add(TNode*, const TActionValue&) {}

  template <class TNode>
  void Apply(const TNode*) {}
};
}  // namespace action
}  // namespace st

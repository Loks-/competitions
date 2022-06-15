#pragma once

namespace ds {
namespace st {
namespace action {
class None {
 public:
  using TSelf = None;
  static const bool modify_data = false;
  static const bool modify_tree = false;

  bool IsEmpty() const { return true; }
  void Clear(){};

  template <class TNode, class TActionValue>
  void Add(TNode*, const TActionValue&) {}

  template <class TNode>
  void Apply(const TNode*) {}
};
}  // namespace action
}  // namespace st
}  // namespace ds

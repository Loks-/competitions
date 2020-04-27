#pragma once

namespace bst {
namespace info {
class None {
 public:
  using TSelf = None;
  static const bool is_none = true;
  static const bool use_data = false;
  static const bool has_size = false;

  template <class TNode>
  void Update(const TNode*) {}
};
}  // namespace info
}  // namespace bst

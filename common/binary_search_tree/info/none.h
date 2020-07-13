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

  // Usused fields to support info with ext trees.
  static const bool support_insert = false;
  static const bool support_remove = false;
};
}  // namespace info
}  // namespace bst

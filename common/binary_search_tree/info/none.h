#pragma once

namespace bst {
namespace info {
class None {
 public:
  using TSelf = None;
  static const bool is_none = true;
  static const bool use_data = false;
  static const bool has_size = false;
  static const bool support_insert = true;
  static const bool support_remove = true;

  template <class TNode>
  void Update(const TNode*) {}

  template <class TNode>
  void Insert(const TNode*) {}

  template <class TNode>
  void Remove(const TNode*) {}
};
}  // namespace info
}  // namespace bst

#pragma once

namespace bst {
namespace info {
class None {
 public:
  using TSelf = None;

  static constexpr bool is_none = true;
  static constexpr bool use_data = false;
  static constexpr bool has_size = false;

  // Usused fields to support info with ext trees.
  static constexpr bool support_insert = false;
  static constexpr bool support_remove = false;

 public:
  constexpr void ClearCreate(unsigned) {}

  template <class TNode>
  constexpr void Update(const TNode*) {}

  // Balance Tree Info
  constexpr void BTIReset() {}

  template <class TNode>
  constexpr void BTICopy(TNode*) {}

  template <class TNode>
  constexpr void BTISwap(TNode*) {}
};
}  // namespace info
}  // namespace bst

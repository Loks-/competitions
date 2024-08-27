#pragma once

namespace ds {
namespace st {
namespace info {
class None {
 public:
  using TSelf = None;

  static constexpr bool is_none = true;
  static constexpr bool use_data = false;

  static constexpr bool use_sinfo_in_update_data = false;
  static constexpr bool use_sinfo_in_update_lr = false;

  static constexpr bool use_update_node = false;

  // To reuse bst function
  static constexpr bool use_keys = false;
  static constexpr bool has_size = false;

 public:
  // External functions should not assume that Update is constexpr.
  template <class TData, class TSInfo>
  constexpr void UpdateData(const TData&, const TSInfo&) {}

  template <class TSInfo>
  constexpr void UpdateLR(const TSelf&, const TSelf&, const TSInfo&) {}

  template <class TNode>
  constexpr void UpdateNode(const TNode*) {}
};
}  // namespace info
}  // namespace st
}  // namespace ds

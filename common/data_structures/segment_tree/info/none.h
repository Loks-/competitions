#pragma once

namespace ds {
namespace st {
namespace info {
class None {
 public:
  using TSelf = None;

  static const bool is_none = true;
  static const bool use_data = false;
  static const bool use_sinfo_in_update_data = false;
  static const bool use_sinfo_in_update_lr = false;

  static const bool use_update_node = false;

  template <class TData, class TSInfo>
  void UpdateData(const TData&, const TSInfo&) {}

  template <class TSInfo>
  void UpdateLR(const TSelf&, const TSelf&, const TSInfo&) {}

  template <class TNode>
  void UpdateNode(const TNode*) {}
};
}  // namespace info
}  // namespace st
}  // namespace ds

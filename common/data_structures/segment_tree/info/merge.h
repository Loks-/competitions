#pragma once

#include "common/template.h"

namespace ds {
namespace st {
namespace info {
template <class TInfo>
inline TInfo MergeLR(const TInfo& l, const TInfo& r) {
  static_assert(
      !TInfo::use_sinfo_in_update_lr,
      "Merge LR doesn't support info with aceess to SInfo for UpdateLR.");
  TInfo t;
  t.UpdateLR(l, r, TEmpty{});
  return t;
}
}  // namespace info
}  // namespace st
}  // namespace ds

#pragma once

namespace st {
namespace info {
template <class TInfo>
inline TInfo MergeLR(const TInfo& l, const TInfo& r) {
  TInfo t;
  t.UpdateLR(l, r);
  return t;
}
}  // namespace info
}  // namespace st

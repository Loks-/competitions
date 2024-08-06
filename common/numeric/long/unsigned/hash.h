#pragma once

#include "common/numeric/long/unsigned.h"
#include "common/stl/hash/vector.h"

namespace std {
template <>
struct hash<numeric::nlong::Unsigned> {
  constexpr size_t operator()(const numeric::nlong::Unsigned& u) const {
    hash<vector<uint32_t>> hv;
    return hv(u.Data());
  }
};
}  // namespace std

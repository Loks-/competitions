#pragma once

#include "common/numeric/long_unsigned.h"
#include "common/stl/hash/vector.h"

namespace std {
template <>
struct hash<LongUnsigned> {
  size_t operator()(const LongUnsigned& u) const {
    hash<vector<uint32_t>> hv;
    return hv(u.Data());
  }
};
}  // namespace std

#pragma once

namespace memory {
class Node {
 public:
  constexpr void ClearCreate(unsigned /* raw_index */) {}
  constexpr void ClearRelease() {}
  constexpr void ClearReuse() {}
};
}  // namespace memory

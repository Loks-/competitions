#pragma once

namespace memory {
class Node {
 public:
  constexpr void ClearCreate() {}
  constexpr void ClearRelease() {}
  constexpr void ClearReuse() {}
};
}  // namespace memory

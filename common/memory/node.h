#pragma once

namespace memory {
class Node {
 public:
  constexpr void clear_create(unsigned /* raw_index */) {}
  constexpr void clear_release() {}
  constexpr void clear_reuse() {}
};
}  // namespace memory

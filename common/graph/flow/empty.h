#pragma once

namespace graph {
namespace flow {
class Empty {
 public:
  constexpr Empty operator-() const { return {}; }
};
}  // namespace flow
}  // namespace graph

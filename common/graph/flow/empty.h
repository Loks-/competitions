#pragma once

namespace graph {
namespace flow {
class Empty {
 public:
  Empty operator-() const { return {}; }
};
}  // namespace flow
}  // namespace graph

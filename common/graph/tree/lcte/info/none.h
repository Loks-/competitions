#pragma once

namespace graph {
namespace lcte {
namespace info {
class PNone {
 public:
  using TSelf = PNone;

  template <class TNode>
  void PUpdate(TNode*) {}

  template <class TNode>
  void Reverse(TNode*) {}
};

class VNone {
 public:
  using TSelf = VNone;

  template <class TNode>
  void VUpdate(TNode*) {}
};
}  // namespace info
}  // namespace lcte
}  // namespace graph

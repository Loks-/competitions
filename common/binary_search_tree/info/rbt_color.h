#pragma once

namespace bst {
namespace info {
template <class TInfo>
class RBTColor : public TInfo {
 public:
  bool black = false;
};
}  // namespace info
}  // namespace bst

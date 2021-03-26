#pragma once

#include <random>

namespace bst {
namespace info {
template <class TTHeight, class TInfo>
class TreapHeight : public TInfo {
 public:
  using THeight = TTHeight;

  THeight treap_height;

  TreapHeight() {
    thread_local std::minstd_rand random_engine;
    treap_height = random_engine();
  }
};
}  // namespace info
}  // namespace bst

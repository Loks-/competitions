#pragma once

#include "common/base.h"

namespace ds {
namespace fus {
// Add Size function to class.
template <class TFUS>
class ProxySize : public TFUS {
 public:
  using TBase = TFUS;

 protected:
  size_t size;

 public:
  ProxySize() { size = 0; }

  void Clear() {
    TBase::Clear();
    size = 0;
  }

  void Init(size_t u) {
    TBase::Init(u);
    size = 0;
  }

  size_t Size() const { return size; }

  void Insert(size_t x) {
    if (!TBase::HasKey(x)) {
      TBase::Insert(x);
      ++size;
    }
  }

  void Delete(size_t x) {
    if (TBase::HasKey(x)) {
      TBase::Delete(x);
      --size;
    }
  }
};
}  // namespace fus
}  // namespace ds

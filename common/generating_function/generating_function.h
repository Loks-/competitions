#pragma once

#include "common/base.h"

#include <memory>

namespace gf {
template <class TValue>
class GeneratingFunction {
 public:
  virtual void Adjust(uint64_t) {}
  virtual TValue Get(uint64_t n) = 0;
};

template <class TValue>
using PGeneratingFunction = std::shared_ptr<GeneratingFunction<TValue>>;
}  // namespace gf

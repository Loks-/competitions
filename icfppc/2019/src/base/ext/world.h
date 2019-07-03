#pragma once

#include "base/core/world.h"
#include "base/ext/map.h"

namespace base {
namespace ext {
class World : public core::World<Map> {
 public:
  TWorker& GetWorker(unsigned windex);
  const TWorker& GetWorker(unsigned windex) const;
};
}  // namespace ext
}  // namespace base

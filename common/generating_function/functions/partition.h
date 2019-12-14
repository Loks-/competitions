#pragma once

#include "common/generating_function/functions/geometric.h"
#include "common/generating_function/generating_function.h"
#include "common/generating_function/operators/partition.h"

namespace gf {
namespace functions {
template <class TValue>
static PGeneratingFunction<TValue> CreatePartition() {
  return gf::operators::CreatePartition(CreatePGeometricOne<TValue>());
}
}  // namespace functions
}  // namespace gf

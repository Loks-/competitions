#pragma once

#include "common/vector/rmq/ppt_mask64.h"

namespace nvector {
template <class TValue>
using RMQ = rmq::PPTMask64<TValue>;
}  // namespace nvector

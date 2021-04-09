#pragma once

#include "common/vector/rmq/mbs64.h"

namespace nvector {
template <class TValue>
using RMQ = rmq::MBS64<TValue>;
}  // namespace nvector

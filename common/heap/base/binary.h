#pragma once

#include "common/heap/base/dheap.h"

#include <functional>

namespace heap {
namespace base {
template <class TData, class TCompare = std::less<TData>>
using Binary = DHeap<2u, TData, TCompare>;
}  // namespace base
}  // namespace heap

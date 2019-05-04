#pragma once

#include "common/heap/dheap.h"
#include <functional>

namespace heap {
template <class TData, class TCompare = std::less<TData>>
using BinaryHeap = DHeap<2u, TData, TCompare>;
}  // namespace heap

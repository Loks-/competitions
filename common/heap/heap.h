#pragma once

#include "common/heap/binary_heap.h"

template <class TValue>
using THeapMax = heap::BinaryHeap<TValue, std::less<TValue>>;
template <class TValue>
using THeapMin = heap::BinaryHeap<TValue, std::greater<TValue>>;

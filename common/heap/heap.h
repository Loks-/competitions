#pragma once

#include "common/heap/binary_heap.h"

template <class TValue>
using HeapMinOnTop = heap::BinaryHeap<TValue, std::less<TValue>>;
template <class TValue>
using HeapMaxOnTop = heap::BinaryHeap<TValue, std::greater<TValue>>;

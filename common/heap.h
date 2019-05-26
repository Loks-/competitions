#pragma once

#include "common/heap/dheap.h"
#include <functional>

template <class TValue>
using HeapMinOnTop = heap::DHeap<4u, TValue, std::less<TValue>>;
template <class TValue>
using HeapMaxOnTop = heap::DHeap<4u, TValue, std::greater<TValue>>;

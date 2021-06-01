#pragma once

#include "common/heap/base/dheap.h"

#include <functional>

template <class TValue>
using HeapMinOnTop = heap::base::DHeap<4u, TValue, std::less<TValue>>;
template <class TValue>
using HeapMaxOnTop = heap::base::DHeap<4u, TValue, std::greater<TValue>>;

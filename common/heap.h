#pragma once

#include "common/heap/base/dheap.h"

#include <functional>

/**
 * @brief A default heap implementation with the minimum element on top.
 *
 * This is an alias for heap::base::DHeap with a branching factor of 4,
 * TValue as the value type, and std::less<TValue> as the comparator.
 *
 * @tparam TValue The type of the values stored in the heap.
 */
template <class TValue>
using HeapMinOnTop = heap::base::DHeap<4u, TValue, std::less<TValue>>;

/**
 * @brief A default heap implementation with the maximum element on top.
 *
 * This is an alias for heap::base::DHeap with a branching factor of 4,
 * TValue as the value type, and std::greater<TValue> as the comparator.
 *
 * @tparam TValue The type of the values stored in the heap.
 */
template <class TValue>
using HeapMaxOnTop = heap::base::DHeap<4u, TValue, std::greater<TValue>>;

#pragma once

#include "common/heap/base/dheap.h"

#include <functional>

/**
 * @brief A default heap implementation with the minimum element on top.
 *
 * This is an alias for heap::base::DHeap with a branching factor of 4,
 * ValueType as the value type, and std::less<ValueType> as the comparator.
 *
 * @tparam ValueType The type of the values stored in the heap.
 */
template <typename ValueType>
using MinHeap = heap::base::DHeap<4u, ValueType, std::less<ValueType>>;

/**
 * @brief A default heap implementation with the maximum element on top.
 *
 * This is an alias for heap::base::DHeap with a branching factor of 4,
 * ValueType as the value type, and std::greater<ValueType> as the comparator.
 *
 * @tparam ValueType The type of the values stored in the heap.
 */
template <typename ValueType>
using MaxHeap = heap::base::DHeap<4u, ValueType, std::greater<ValueType>>;

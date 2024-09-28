#pragma once

#include "common/base.h"

#include <array>
#include <utility>

namespace hidden {

/**
 * @brief Helper function to create an array with all elements set to a given value.
 *
 * This function uses an index sequence to initialize each element of the array
 * with the provided value.
 *
 * @tparam TValue The type of the elements in the array.
 * @tparam Is The index sequence for the array elements.
 * @param value The value to set for each element of the array.
 * @param An index sequence used to iterate over the array elements.
 * @return A std::array with all elements set to the provided value.
 */
template <class TValue, std::size_t... Is>
constexpr std::array<TValue, sizeof...(Is)> MakeArray(
    const TValue& value, std::index_sequence<Is...>) {
  return {{(static_cast<void>(Is), value)...}};
}

} // namespace hidden

/**
 * @brief Creates an array of size N with all elements set to a given value.
 *
 * This function creates a std::array of the specified size and initializes
 * each element with the provided value.
 *
 * @tparam TValue The type of the elements in the array.
 * @tparam N The size of the array.
 * @param value The value to set for each element of the array.
 * @return A std::array of size N with all elements set to the provided value.
 */
template <class TValue, size_t N>
constexpr std::array<TValue, N> MakeArray(const TValue& value) {
  return hidden::MakeArray(value, std::make_index_sequence<N>());
}

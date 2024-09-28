#pragma once

#include <iostream>
#include <tuple>
#include <utility>

/**
 * @brief Helper function to read elements of a tuple from an input stream.
 *
 * This function reads each element of the tuple from the input stream.
 *
 * @tparam Tuple The type of the tuple.
 * @tparam Index The index sequence for the tuple elements.
 * @param s The input stream from which to read the tuple elements.
 * @param t The tuple to be read from the input stream.
 * @return The input stream after reading the tuple elements.
 */
template <typename Tuple, std::size_t... Index>
inline std::istream& ReadTuple(std::istream& s, Tuple& t, std::index_sequence<Index...>) {
    ((s >> std::get<Index>(t)), ...);
    return s;
}

/**
 * @brief Overload of the extraction operator for std::tuple.
 *
 * This function reads a tuple of values from an input stream. It extracts each
 * element of the tuple from the stream.
 *
 * @tparam Types The types of the elements of the tuple.
 * @param s The input stream from which to read the tuple.
 * @param t The tuple to be read from the input stream.
 * @return The input stream after reading the tuple.
 */
template <typename... Types>
inline std::istream& operator>>(std::istream& s, std::tuple<Types...>& t) {
    return ReadTuple(s, t, std::index_sequence_for<Types...>{});
}

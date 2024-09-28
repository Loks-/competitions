#pragma once

#include <tuple>

namespace std {

/**
 * @brief Adds the elements of the right-hand tuple to the left-hand tuple.
 *
 * This operator overloads the += operator for std::tuple, allowing element-wise addition.
 *
 * @tparam Tuple The type of the tuple.
 * @param l The left-hand tuple to which the right-hand tuple will be added.
 * @param r The right-hand tuple whose elements will be added to the left-hand tuple.
 * @return A reference to the modified left-hand tuple.
 */
template <typename... Types>
constexpr std::tuple<Types...>& operator+=(std::tuple<Types...>& l, const std::tuple<Types...>& r) {
    std::apply([&](auto&... l_elem) {
        std::apply([&](const auto&... r_elem) {
            ((l_elem += r_elem), ...);
        }, r);
    }, l);
    return l;
}

/**
 * @brief Subtracts the elements of the right-hand tuple from the left-hand tuple.
 *
 * This operator overloads the -= operator for std::tuple, allowing element-wise subtraction.
 *
 * @tparam Tuple The type of the tuple.
 * @param l The left-hand tuple from which the right-hand tuple will be subtracted.
 * @param r The right-hand tuple whose elements will be subtracted from the left-hand tuple.
 * @return A reference to the modified left-hand tuple.
 */
template <typename... Types>
constexpr std::tuple<Types...>& operator-=(std::tuple<Types...>& l, const std::tuple<Types...>& r) {
    std::apply([&](auto&... l_elem) {
        std::apply([&](const auto&... r_elem) {
            ((l_elem -= r_elem), ...);
        }, r);
    }, l);
    return l;
}

/**
 * @brief Returns a new tuple that is the sum of the elements of the two input tuples.
 *
 * This operator overloads the + operator for std::tuple, allowing element-wise addition.
 *
 * @tparam Tuple The type of the tuple.
 * @param l The left-hand tuple to be added.
 * @param r The right-hand tuple to be added.
 * @return A new tuple containing the element-wise sum of the input tuples.
 */
template <typename... Types>
constexpr std::tuple<Types...> operator+(const std::tuple<Types...>& l, const std::tuple<Types...>& r) {
    return std::apply([&](const auto&... l_elem) {
        return std::apply([&](const auto&... r_elem) {
            return std::make_tuple((l_elem + r_elem)...);
        }, r);
    }, l);
}

/**
 * @brief Returns a new tuple that is the difference of the elements of the two input tuples.
 *
 * This operator overloads the - operator for std::tuple, allowing element-wise subtraction.
 *
 * @tparam Tuple The type of the tuple.
 * @param l The left-hand tuple to be subtracted from.
 * @param r The right-hand tuple to be subtracted.
 * @return A new tuple containing the element-wise difference of the input tuples.
 */
template <typename... Types>
constexpr std::tuple<Types...> operator-(const std::tuple<Types...>& l, const std::tuple<Types...>& r) {
    return std::apply([&](const auto&... l_elem) {
        return std::apply([&](const auto&... r_elem) {
            return std::make_tuple((l_elem - r_elem)...);
        }, r);
    }, l);
}

}  // namespace std

#pragma once

#include <vector>

namespace nvector {
namespace hidden {

/**
 * @brief Helper class template to define a multidimensional vector type.
 *
 * This class template recursively defines a multidimensional vector type.
 *
 * @tparam T The type of the elements in the vector.
 * @tparam d The number of dimensions.
 */
template <class T, unsigned d>
class HDVector;

template <class T>
class HDVector<T, 0u> {
 public:
  using TType = T;
};

template <class T, unsigned d>
class HDVector {
 public:
  using TType = std::vector<typename HDVector<T, d - 1>::TType>;
};

}  // namespace hidden

/**
 * @brief Alias template for a multidimensional vector type.
 *
 * This alias template simplifies the usage of the HDVector class template.
 *
 * @tparam T The type of the elements in the vector.
 * @tparam d The number of dimensions.
 */
template <class T, unsigned d>
using DVector = typename hidden::HDVector<T, d>::TType;

/**
 * @brief Helper function to create a multidimensional vector.
 *
 * This function recursively creates a multidimensional vector with the
 * specified dimensions and fills it with the given value.
 *
 * @tparam T The type of the elements in the vector.
 * @param value The value to fill the vector with.
 * @return The value itself, used as the base case for recursion.
 */
template <class T>
constexpr T MakeV(const T& value) {
  return value;
}

/**
 * @brief Helper function to create a multidimensional vector.
 *
 * This function recursively creates a multidimensional vector with the
 * specified dimensions and fills it with the given value.
 *
 * @tparam T The type of the elements in the vector.
 * @tparam Ds The remaining dimensions of the vector.
 * @param value The value to fill the vector with.
 * @param d0 The size of the current dimension.
 * @param ds The sizes of the remaining dimensions.
 * @return A multidimensional vector with the specified dimensions and filled
 * with the value.
 */
template <class T, typename... Ds>
constexpr auto MakeV(const T& value, unsigned d0, Ds... ds) {
  return DVector<T, sizeof...(Ds) + 1>(d0, MakeV<T>(value, ds...));
}

/**
 * @brief Creates a multidimensional vector with the specified dimensions and
 * fills it with the default value.
 *
 * This function creates a multidimensional vector with the specified dimensions
 * and fills it with the default value of the specified type.
 *
 * @tparam T The type of the elements in the vector.
 * @tparam Ds The dimensions of the vector.
 * @param ds The sizes of the dimensions.
 * @return A multidimensional vector with the specified dimensions and filled
 * with the default value.
 */
template <class T, typename... Ds>
constexpr auto Make(Ds... ds) {
  return MakeV(T{}, ds...);
}

}  // namespace nvector

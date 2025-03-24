#pragma once

#include <tuple>
#include <type_traits>

namespace templates {

/**
 * @brief Helper type trait to check if a type exists in tuple.
 * Primary template is not defined - only the specialization for std::tuple is
 * used.
 *
 * @tparam T The type to check for.
 * @tparam Tuple The tuple type to search in.
 */
template <typename T, typename Tuple>
struct HasType;

/**
 * @brief Specialization that unpacks the tuple's types into Types...
 * Uses fold expression to check if T matches any of the tuple's types.
 *
 * @tparam T The type to check for.
 * @tparam Types The parameter pack of types from the tuple.
 */
template <typename T, typename... Types>
struct HasType<T, std::tuple<Types...>> {
  /**
   * @brief Result of the type check.
   * True if T is present in the tuple's types, false otherwise.
   */
  static constexpr bool value = (std::is_same_v<T, Types> || ...);
};

/**
 * @brief Helper type trait to append a type to a tuple.
 * Primary template is not defined - only the specialization for std::tuple is
 * used.
 *
 * @tparam T The type to append.
 * @tparam Tuple The tuple type to append to.
 */
template <typename T, typename Tuple>
struct Append;

/**
 * @brief Specialization that unpacks the tuple's types and creates a new tuple
 * with T appended.
 *
 * @tparam T The type to append.
 * @tparam Types The parameter pack of types from the original tuple.
 */
template <typename T, typename... Types>
struct Append<T, std::tuple<Types...>> {
  /**
   * @brief The resulting tuple type with T appended.
   */
  using type = std::tuple<Types..., T>;
};

/**
 * @brief Helper alias template for Append.
 *
 * @tparam T The type to append.
 * @tparam Tuple The tuple type to append to.
 */
template <typename T, typename Tuple>
using AppendT = typename Append<T, Tuple>::type;

/**
 * @brief Helper type trait to prepend a type to a tuple.
 * Primary template is not defined - only the specialization for std::tuple is
 * used.
 *
 * @tparam T The type to prepend.
 * @tparam Tuple The tuple type to prepend to.
 */
template <typename T, typename Tuple>
struct Prepend;

/**
 * @brief Specialization that unpacks the tuple's types and creates a new tuple
 * with T prepended.
 *
 * @tparam T The type to prepend.
 * @tparam Types The parameter pack of types from the original tuple.
 */
template <typename T, typename... Types>
struct Prepend<T, std::tuple<Types...>> {
  /**
   * @brief The resulting tuple type with T prepended.
   */
  using type = std::tuple<T, Types...>;
};

/**
 * @brief Helper alias template for Prepend.
 *
 * @tparam T The type to prepend.
 * @tparam Tuple The tuple type to prepend to.
 */
template <typename T, typename Tuple>
using PrependT = typename Prepend<T, Tuple>::type;

/**
 * @brief Helper type trait to append a type to a tuple only if it's not already
 * present.
 *
 * @tparam T The type to append if missing.
 * @tparam Tuple The tuple type to append to.
 */
template <typename T, typename Tuple>
struct AppendIfMissing {
  /**
   * @brief The resulting tuple type.
   * If T is already in the tuple, returns the original tuple.
   * Otherwise, returns a new tuple with T appended.
   */
  using type =
      std::conditional_t<HasType<T, Tuple>::value, Tuple, AppendT<T, Tuple>>;
};

/**
 * @brief Helper alias template for AppendIfMissing.
 *
 * @tparam T The type to append if missing.
 * @tparam Tuple The tuple type to append to.
 */
template <typename T, typename Tuple>
using AppendIfMissingT = typename AppendIfMissing<T, Tuple>::type;

/**
 * @brief Helper type trait to prepend a type to a tuple only if it's not
 * already present.
 *
 * @tparam T The type to prepend if missing.
 * @tparam Tuple The tuple type to prepend to.
 */
template <typename T, typename Tuple>
struct PrependIfMissing {
  /**
   * @brief The resulting tuple type.
   * If T is already in the tuple, returns the original tuple.
   * Otherwise, returns a new tuple with T prepended.
   */
  using type =
      std::conditional_t<HasType<T, Tuple>::value, Tuple, PrependT<T, Tuple>>;
};

/**
 * @brief Helper alias template for PrependIfMissing.
 *
 * @tparam T The type to prepend if missing.
 * @tparam Tuple The tuple type to prepend to.
 */
template <typename T, typename Tuple>
using PrependIfMissingT = typename PrependIfMissing<T, Tuple>::type;

}  // namespace templates

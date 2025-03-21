#pragma once

/**
 * @brief An empty class used as a placeholder.
 *
 * This class is useful for template specialization and SFINAE techniques
 * where a minimal type is needed.
 */
class TEmpty {};

/**
 * @brief A template class used to represent a boolean constant in template
 * programming.
 *
 * @tparam B The boolean value.
 */
template <bool B>
class TMetaBool {};

/**
 * @brief Alias for TMetaBool<false>.
 *
 * Provides a convenient way to represent false in template metaprogramming.
 */
using TMetaFalse = TMetaBool<false>;

/**
 * @brief Alias for TMetaBool<true>.
 *
 * Provides a convenient way to represent true in template metaprogramming.
 */
using TMetaTrue = TMetaBool<true>;

/**
 * @brief A template class used to represent an unsigned integer constant in
 * template programming.
 *
 * @tparam N The unsigned integer value.
 */
template <unsigned N>
class TMetaUnsigned {};

/**
 * @brief A template class used to wrap a type in template programming.
 *
 * @tparam T The type.
 */
template <typename T>
class TMetaType {};

/**
 * @brief A function used to suppress unused variable warnings in debug builds
 * while allowing release builds to optimize them away.
 *
 * This function is particularly useful when dealing with debug-only variables
 * or parameters that are only used in debug builds.
 *
 * @tparam T The type of the parameter.
 * @param value The parameter to use.
 */
template <typename T>
constexpr void MetaUse([[maybe_unused]] const T& value) {}

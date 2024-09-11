#pragma once

/**
 * @brief An empty class used as a placeholder.
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
 */
using TMetaFalse = TMetaBool<false>;

/**
 * @brief Alias for TMetaBool<true>.
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
template <class T>
class TMetaType {};

/**
 * @brief A function used to avoid compile warnings if a parameter is used in
 * debug settings but not in release settings.
 *
 * @tparam T The type of the parameter.
 * @param value The parameter to use.
 */
template <class T>
constexpr void MetaUse(const T&) {}

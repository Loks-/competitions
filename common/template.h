#pragma once

/**
 * @brief An empty class used as a placeholder.
 *
 * This class is useful for template specialization and SFINAE techniques
 * where a minimal type is needed.
 */
class MetaEmpty {};

/**
 * @brief A template class used to represent a boolean constant in template
 * programming.
 *
 * @tparam Value The boolean value.
 */
template <bool Value>
class MetaBool {};

/**
 * @brief Alias for MetaBool<false>.
 *
 * Provides a convenient way to represent false in template metaprogramming.
 */
using MetaFalse = MetaBool<false>;

/**
 * @brief Alias for MetaBool<true>.
 *
 * Provides a convenient way to represent true in template metaprogramming.
 */
using MetaTrue = MetaBool<true>;

/**
 * @brief A template class used to represent an unsigned integer constant in
 * template programming.
 *
 * @tparam Value The unsigned integer value.
 */
template <unsigned Value>
class MetaUnsigned {};

/**
 * @brief A template class used to wrap a type in template programming.
 *
 * @tparam Type The type to wrap.
 */
template <typename Type>
class MetaType {};

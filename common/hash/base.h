#pragma once

namespace nhash {

/**
 * @brief Forward declaration of the Hash class template.
 *
 * This forward declaration allows for the definition of custom hash functions
 * for different policies and types without including the full definition of the
 * Hash class.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T The type of the object to be hashed.
 */
template <class Policy, class T>
class Hash;

}  // namespace nhash

#pragma once

namespace nhash {
/**
 * @brief Enum representing different integral type policies.
 */
enum class EPolicyIntegralTypes { IDENTITY, MIX };

/**
 * @brief Enum representing different combine policies.
 */
enum class EPolicyCombine { XOR, MIX };

/**
 * @brief Meta class for integral type policies.
 *
 * This class is a placeholder for different integral type policies.
 *
 * @tparam Policy The integral type policy.
 */
template <EPolicyIntegralTypes>
class TMetaIntegralTypes {};

/**
 * @brief Meta class for combine policies.
 *
 * This class is a placeholder for different combine policies.
 *
 * @tparam Policy The combine policy.
 */
template <EPolicyCombine>
class TMetaCombine {};

/**
 * @brief Policy class combining integral type and combine policies.
 *
 * This class defines a policy by combining an integral type policy and a
 * combine policy.
 *
 * @tparam integral_types_ The integral type policy.
 * @tparam combine_ The combine policy.
 */
template <EPolicyIntegralTypes integral_types_, EPolicyCombine combine_>
class Policy {
 public:
  static constexpr EPolicyIntegralTypes integral_types = integral_types_;
  static constexpr EPolicyCombine combine = combine_;
};

/**
 * @brief Proxy helper for integral type identity policy.
 *
 * This alias defines a policy with the IDENTITY integral type policy and the
 * combine policy of TPolicy.
 *
 * @tparam TPolicy The policy to proxy.
 */
template <class TPolicy>
using TProxyHelperITI =
    Policy<EPolicyIntegralTypes::IDENTITY, TPolicy::combine>;

/**
 * @brief Fast policy using IDENTITY integral type and XOR combine.
 */
using PolicyFast = Policy<EPolicyIntegralTypes::IDENTITY, EPolicyCombine::XOR>;

/**
 * @brief Safe policy using MIX integral type and MIX combine.
 */
using PolicySafe = Policy<EPolicyIntegralTypes::MIX, EPolicyCombine::MIX>;
}  // namespace nhash

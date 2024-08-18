#pragma once

namespace nhash {
enum class EPolicyIntegralTypes { IDENTITY, MIX };

enum class EPolicyCombine { XOR, MIX };

template <EPolicyIntegralTypes>
class TFakeIntegralTypes {};

template <EPolicyCombine>
class TFakeCombine {};

template <EPolicyIntegralTypes integral_types_, EPolicyCombine combine_>
class Policy {
 public:
  static constexpr EPolicyIntegralTypes integral_types = integral_types_;
  static constexpr EPolicyCombine combine = combine_;
};

template <class TPolicy>
using TProxyHelperITI =
    Policy<EPolicyIntegralTypes::IDENTITY, TPolicy::combine>;

using PolicyFast = Policy<EPolicyIntegralTypes::IDENTITY, EPolicyCombine::XOR>;
using PolicySafe = Policy<EPolicyIntegralTypes::MIX, EPolicyCombine::MIX>;
}  // namespace nhash

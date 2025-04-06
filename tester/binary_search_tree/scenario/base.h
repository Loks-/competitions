#pragma once

#include "tester/binary_search_tree/scenario/result.h"

#include <chrono>
#include <string>
#include <string_view>

namespace tester {
namespace bst {
namespace scenario {

template <typename Derived>
class Base {
 public:
  template <bool extra_checks, template <typename, typename, typename,
                                         typename> class Implementation>
  static Result run(size_t size) {
    Result result;
    result.scenario_id = Derived::id();

    try {
      auto [hash, duration] =
          Derived::template run_impl<extra_checks, Implementation>(
              size, result.implementation_id);
      result.control_hash = hash;
      result.duration = duration;
      result.success = true;
    } catch (const std::exception& e) {
      result.success = false;
      result.control_hash = 0;
      result.error_message = e.what();
      result.duration = std::chrono::nanoseconds(0);
    }

    return result;
  }
};

}  // namespace scenario
}  // namespace bst
}  // namespace tester

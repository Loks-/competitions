#pragma once

#include "common/base.h"

#include <chrono>
#include <string>

namespace tester {
namespace bst {
namespace scenario {

struct Result {
  std::string scenario_id;
  std::string implementation_id;
  bool success;
  std::string error_message;
  size_t control_hash;
  std::chrono::nanoseconds duration;
};

}  // namespace scenario
}  // namespace bst
}  // namespace tester

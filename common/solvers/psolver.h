#pragma once

#include <memory>

namespace solvers {
class Base;

using PSolver = std::shared_ptr<Base>;
}  // namespace solvers

#pragma once

#include "base/action.h"
#include <ostream>

namespace base {
class Solution;

std::ostream& operator<<(std::ostream& os, const Action& a);
std::ostream& operator<<(std::ostream& os, const ActionsList& al);
std::ostream& operator<<(std::ostream& os, const Solution& s);
}  // namespace base

#pragma once

#include "base/action.h"
#include "base/point.h"
#include "base/solution.h"
#include <string>

namespace base {
Point DecodePoint(const std::string& s);
ActionsList DecodeActionList(const std::string& s);
Solution DecodeSolution(const std::string& s);
}  // namespace base

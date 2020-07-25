#pragma once

#include <string>

namespace app {
std::string EncodeMessage(const std::string& s);
std::string DecodeMessage(const std::string& s);
}  // namespace app

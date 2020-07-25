#pragma once

#include "message.h"

#include <string>

namespace tournament {
// Faster version without dependency on galaxy.
// We also use different format for messages for simper code.
std::string EncodeMessage(const Message& m);
Message DecodeMessage(const std::string& s);
Message Send(const Message& m);
}  // namespace tournament

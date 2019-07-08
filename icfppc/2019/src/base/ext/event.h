#pragma once

#include "base/ext/event_type.h"

namespace base {
namespace ext {
class Event {
 public:
  int time;
  EventType type;
  unsigned location;
};
}  // namespace ext
}  // namespace base

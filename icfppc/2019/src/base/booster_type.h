#pragma once

namespace base {
enum class BoosterType {
  NONE = 0,
  EXTENSION,
  FAST_WHEELS,
  DRILL,
  TELEPORT,
  CLONING,
  UNKNOWN
};

const int TIME_FAST_WHEELS = 50;
const int TIME_DRILL = 30;
}  // namespace base

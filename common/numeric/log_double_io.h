#pragma once

#include "common/numeric/log_double.h"
#include <istream>
#include <ostream>

inline std::ostream& operator<<(std::ostream& s, const LogDouble& d) {
  return s << d.Get();
}

inline std::istream& operator>>(std::istream& s, LogDouble& d) {
  double t;
  s >> t;
  d = t;
  return s;
}

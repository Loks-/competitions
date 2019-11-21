#pragma once

#include "common/modular/static/bool.h"
#include <istream>
#include <ostream>

namespace modular {
namespace mstatic {
inline std::ostream& operator<<(std::ostream& s, const Bool& m) {
  return s << m.Get();
}

inline std::istream& operator>>(std::istream& s, Bool& m) {
  int64_t t;
  s >> t;
  m.SetS(t);
  return s;
}
}  // namespace mstatic
}  // namespace modular

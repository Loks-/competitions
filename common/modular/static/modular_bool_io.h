#pragma once

#include "modular_bool.h"
#include <istream>
#include <ostream>

inline std::ostream& operator<<(std::ostream& s, const ModularBool& m) {
  s << m.Get();
  return s;
}

inline std::istream& operator>>(std::istream& s, ModularBool& m) {
  int64_t t;
  s >> t;
  m.SetS(t);
  return s;
}

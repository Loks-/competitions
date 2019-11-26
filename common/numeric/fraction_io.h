#pragma once

#include "common/numeric/fraction.h"
#include <ostream>

namespace numeric {
template <class TValue>
inline std::ostream& operator<<(std::ostream& s, const Fraction<TValue>& f) {
  return s << f.GetN() << "/" << f.GetD();
}
}  // namespace numeric

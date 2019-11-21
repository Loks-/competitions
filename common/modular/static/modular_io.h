#pragma once

#include "common/modular/static/modular.h"
#include <istream>
#include <ostream>

namespace modular {
namespace mstatic {
template <uint64_t mod, bool is_prime, bool is_32bit>
inline std::ostream& operator<<(std::ostream& s,
                                const Modular<mod, is_prime, is_32bit>& m) {
  return s << m.Get();
}

template <uint64_t mod, bool is_prime, bool is_32bit>
inline std::istream& operator>>(std::istream& s,
                                Modular<mod, is_prime, is_32bit>& m) {
  int64_t t;
  s >> t;
  m.SetS(t);
  return s;
}
}  // namespace mstatic
}  // namespace modular

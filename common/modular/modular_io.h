#pragma once

#include "modular.h"
#include <istream>
#include <ostream>

template<uint64_t mod, bool is_prime, bool is_32bit>
inline std::ostream& operator<<(std::ostream& s, const Modular<mod, is_prime, is_32bit>& m) { s << m.Get(); return s; }

template<uint64_t mod, bool is_prime, bool is_32bit>
inline std::istream& operator>>(std::istream& s, Modular<mod, is_prime, is_32bit>& m) { int64_t t; s >> t; m.SetS(t); return s; }

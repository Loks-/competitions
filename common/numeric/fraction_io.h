#pragma once

#include "fraction.h"
#include <ostream>

template<class TValue>
inline std::ostream& operator<<(std::ostream& s, const Fraction<TValue>& f) { s << f.GetN() << "/" << f.GetD(); return s; }

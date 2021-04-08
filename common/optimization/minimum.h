#pragma once

#include "common/optimization/minimum/golden_section_search.h"

namespace opt {
// Local minimum for continuous function.
// Global minimum for unimodal function.
template <class TFunction>
inline double Minimum(TFunction& f, double l, double r, double eps) {
  return minimum::GoldenSectionSearch(f, l, r, eps);
}
}  // namespace opt

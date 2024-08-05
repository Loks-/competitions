#pragma once

#include "common/geometry/d2/vector.h"
#include "common/numeric/utils/sign.h"

template <class T>
constexpr geometry::d2::Vector<T> Sign(const geometry::d2::Vector<T>& v) {
  return {Sign(v.dx), Sign(v.dy)};
}

#pragma once

#include "common/base.h"
#include "common/geometry/d3/point.h"
#include "common/geometry/d3/vector.h"

#include <vector>

template <class T>
constexpr geometry::d3::Point<T> MapCoordinates(
    const geometry::d3::Point<T>& p, const std::vector<unsigned>& vmap) {
  assert(vmap.size() >= 3);
  return { p[vmap[0]], p[vmap[1]], p[vmap[2]]; }
}

template <class T>
constexpr geometry::d3::Vector<T> MapCoordinates(
    const geometry::d3::Vector<T>& v, const std::vector<unsigned>& vmap) {
  assert(vmap.size() >= 3);
  return { v[vmap[0]], v[vmap[1]], v[vmap[2]]; }
}

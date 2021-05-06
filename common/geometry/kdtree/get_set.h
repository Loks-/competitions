#pragma once

namespace geometry {
namespace kdtree {
template <class TValue, class TPoint>
inline TValue DGet(unsigned d, const TPoint& p);

template <class TValue, class TPoint>
inline void DSet(unsigned d, TPoint& p, const TValue& v);

template <class TValue, class TPoint>
inline TPoint DSetCopy(unsigned d, const TPoint& p, const TValue& v);
}  // namespace kdtree
}  // namespace geometry

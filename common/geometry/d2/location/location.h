#pragma once

namespace geometry {
namespace d2 {
namespace location {
class Location {
 public:
  enum Type {
    INSIDE, EDGE, VERTEX, OUTSIDE
  };

  Type type;
  // index of vertex of edge if location is EDGE/VERTEX
  // on polygon
  unsigned index; 
};
}  // namespace location
}  // namespace d2
}  // namespace geometry

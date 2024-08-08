#pragma once

#include "common/geometry/d2/axis/rectangle.h"
#include "common/geometry/d2/utils/neighbors.h"
#include "common/graph/graph.h"

constexpr DirectedGraph NeighborsDGraphD2(unsigned dx, unsigned dy) {
  const I2ARectangle b({0, 0}, {dx - 1, dy - 1});
  DirectedGraph g(dx * dy);
  I2Point p(0, 0);
  for (p.y = 0; p.y < dy; ++p.y) {
    for (p.x = 0; p.x < dx; ++p.x) {
      for (auto p1 : I2NeighborsD2(p)) {
        if (b.Inside(p1)) g.AddEdge(b.InsideIndexYX(p), b.InsideIndexYX(p1));
      }
    }
  }
  return g;
}

constexpr DirectedGraph NeighborsDGraphD3(unsigned dx, unsigned dy) {
  const I2ARectangle b({0, 0}, {dx - 1, dy - 1});
  DirectedGraph g(dx * dy);
  I2Point p(0, 0);
  for (p.y = 0; p.y < dy; ++p.y) {
    for (p.x = 0; p.x < dx; ++p.x) {
      for (auto p1 : I2NeighborsD3(p)) {
        if (b.Inside(p1)) g.AddEdge(b.InsideIndexYX(p), b.InsideIndexYX(p1));
      }
    }
  }
  return g;
}

constexpr DirectedGraph NeighborsDGraphD4(unsigned dx, unsigned dy) {
  const I2ARectangle b({0, 0}, {dx - 1, dy - 1});
  DirectedGraph g(dx * dy);
  I2Point p(0, 0);
  for (p.y = 0; p.y < dy; ++p.y) {
    for (p.x = 0; p.x < dx; ++p.x) {
      for (auto p1 : I2NeighborsD4(p)) {
        if (b.Inside(p1)) g.AddEdge(b.InsideIndexYX(p), b.InsideIndexYX(p1));
      }
    }
  }
  return g;
}

constexpr DirectedGraph NeighborsDGraphD8(unsigned dx, unsigned dy) {
  const I2ARectangle b({0, 0}, {dx - 1, dy - 1});
  DirectedGraph g(dx * dy);
  I2Point p(0, 0);
  for (p.y = 0; p.y < dy; ++p.y) {
    for (p.x = 0; p.x < dx; ++p.x) {
      for (auto p1 : I2NeighborsD8(p)) {
        if (b.Inside(p1)) g.AddEdge(b.InsideIndexYX(p), b.InsideIndexYX(p1));
      }
    }
  }
  return g;
}

constexpr UndirectedGraph NeighborsUGraphD4(unsigned dx, unsigned dy) {
  const I2ARectangle b({0, 0}, {dx - 1, dy - 1});
  UndirectedGraph g(dx * dy);
  I2Point p(0, 0);
  for (p.y = 0; p.y < dy; ++p.y) {
    for (p.x = 0; p.x < dx; ++p.x) {
      for (auto p1 : I2NeighborsD2(p)) {
        if (b.Inside(p1)) g.AddEdge(b.InsideIndexYX(p), b.InsideIndexYX(p1));
      }
    }
  }
  return g;
}

constexpr UndirectedGraph NeighborsUGraphD8(unsigned dx, unsigned dy) {
  const I2ARectangle b({0, 0}, {dx - 1, dy - 1});
  UndirectedGraph g(dx * dy);
  I2Point p(0, 0);
  for (p.y = 0; p.y < dy; ++p.y) {
    for (p.x = 0; p.x < dx; ++p.x) {
      const auto i0 = b.InsideIndexYX(p);
      for (auto p1 : I2NeighborsD8(p)) {
        if (!b.Inside(p1)) continue;
        const auto i1 = b.InsideIndexYX(p1);
        if (i1 > i0) g.AddEdge(i0, i1);
      }
    }
  }
  return g;
}

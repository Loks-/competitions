#pragma once

#include <vector>

namespace graph {
template <class TEdgeInfo>
class Edge {
 public:
  unsigned to;
  TEdgeInfo info;
};

template <class TEdgeInfo>
class EdgeConstIterator {
 public:
  using TSelf = EdgeConstIterator<TEdgeInfo>;

 protected:
  const unsigned* p1;
  const TEdgeInfo* p2;

 public:
  constexpr EdgeConstIterator(const unsigned* _p1, const TEdgeInfo* _p2)
      : p1(_p1), p2(_p2) {}

  constexpr Edge<TEdgeInfo> operator*() const { return {*p1, *p2}; }

  constexpr EdgeConstIterator& operator++() {
    ++p1;
    ++p2;
    return *this;
  }

  constexpr bool operator!=(const TSelf& r) const { return p1 != r.p1; }
};

template <class TEdgeInfo>
class EdgesFromVertex {
 public:
  using const_iterator = EdgeConstIterator<TEdgeInfo>;

 public:
  const std::vector<unsigned>& to;
  const std::vector<TEdgeInfo>& info;

 public:
  constexpr EdgesFromVertex(const std::vector<unsigned>& _to,
                            const std::vector<TEdgeInfo>& _info)
      : to(_to), info(_info) {}

  constexpr unsigned Size() const { return unsigned(to.size()); }

  constexpr const_iterator begin() const {
    return const_iterator(&to.front(), &info.front());
  }

  constexpr const_iterator end() const {
    return const_iterator(&to.front() + Size(), &info.front() + Size());
  }
};
}  // namespace graph

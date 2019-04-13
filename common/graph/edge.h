#pragma once

#include <vector>

namespace graph {
template <class TEdgeInfo>
class GraphEdge {
 public:
  unsigned to;
  TEdgeInfo info;
};

template <class TEdgeInfo>
class GraphEdgeConstIterator {
 public:
  using TSelf = GraphEdgeConstIterator<TEdgeInfo>;

 protected:
  const unsigned* p1;
  const TEdgeInfo* p2;

 public:
  GraphEdgeConstIterator(const unsigned* _p1, const TEdgeInfo* _p2)
      : p1(_p1), p2(_p2) {}

  GraphEdge<TEdgeInfo> operator*() const { return {*p1, *p2}; }

  GraphEdgeConstIterator& operator++() {
    ++p1;
    ++p2;
    return *this;
  }

  bool operator!=(const TSelf& r) const { return p1 != r.p1; }
};

template <class TEdgeInfo>
class GraphEdgesFromVertex {
 public:
  using const_iterator = GraphEdgeConstIterator<TEdgeInfo>;

  const std::vector<unsigned>& to;
  const std::vector<TEdgeInfo>& info;

  GraphEdgesFromVertex(const std::vector<unsigned>& _to,
                       const std::vector<TEdgeInfo>& _info)
      : to(_to), info(_info) {}

  unsigned Size() const { return unsigned(to.size()); }

  const_iterator begin() const {
    return const_iterator(&to.front(), &info.front());
  }

  const_iterator end() const {
    return const_iterator(&to.front() + Size(), &info.front() + Size());
  }
};
}  // namespace graph

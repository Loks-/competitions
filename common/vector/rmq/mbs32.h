#pragma once

#include "common/base.h"
#include "common/numeric/bits/first_bit.h"
#include "common/vector/rmq/position_value.h"
#include "common/vector/rmq/ppt.h"

#include <stack>
#include <utility>
#include <vector>

namespace nvector {
namespace rmq {
// Similar to PPT but use binary mask search on small blocks of size 32.
// O(N log N / block_size + N) memory, O(N log N / block_size + N) preprocessing
// time, O(1) request time
template <class TTValue>
class MBS32 {
 public:
  using TValue = TTValue;
  using TPositionValue = PositionValue<TValue>;
  static const unsigned k = 32;
  using TMask = uint32_t;
  static const TMask one = 1;

 protected:
  size_t n, l;
  std::vector<TMask> vmask;
  std::vector<size_t> vlpos;
  std::vector<TValue> vv;
  PPT<TValue> rmq;

 protected:
  TPositionValue GetBlock(size_t j, size_t b, size_t e) const {
    auto m = vmask[j * k + e - 1];
    m &= -(one << b);
    size_t pos = j * k + numeric::Trailing0Bits(m);
    return {pos, vv[pos]};
  }

 public:
  MBS32() {}
  MBS32(const std::vector<TValue>& v) { Build(v); }

  void Build(const std::vector<TValue>& v) {
    n = v.size();
    l = (n + k - 1) / k;
    n = k * l;
    vv = v;
    for (; vv.size() < n;) vv.push_back(v.back());
    vmask.resize(n);
    vlpos.resize(l);
    std::vector<TValue> vlmin(l);
    std::stack<std::pair<TValue, size_t>> s;
    for (size_t i = 0; i < l; ++i) {
      size_t ik = i * k, lpos = ik;
      TValue lvalue = vv[ik];
      TMask mask = one;
      vmask[ik] = mask;
      s.push({lvalue, 0});
      for (size_t j = 1; j < k; ++j) {
        mask |= (one << j);
        auto& vj = vv[ik + j];
        if (vj < lvalue) {
          for (; !s.empty();) s.pop();
          mask = (one << j);
          lpos = ik + j;
          lvalue = vv[lpos];
        } else {
          for (; vj < s.top().first; s.pop()) mask ^= (one << s.top().second);
        }
        vmask[ik + j] = mask;
        s.push({vj, j});
      }
      vlpos[i] = lpos;
      vlmin[i] = lvalue;
    }
    rmq.Build(vlmin);
  }

  TPositionValue Minimum(size_t b, size_t e) const {
    assert(b < e);
    size_t ib = b / k, jb = b % k, ie = e / k, je = e % k;
    if (je == 0) {
      ie -= 1;
      je = k;
    }
    if (ib == ie) {
      return GetBlock(ib, jb, je);
    } else if (ib + 1 == ie) {
      return Merge(GetBlock(ib, jb, k), GetBlock(ie, 0, je));
    } else {
      auto pv = rmq.Minimum(ib + 1, ie);
      pv.pos = vlpos[pv.pos];
      return Merge(Merge(GetBlock(ib, jb, k), pv), GetBlock(ie, 0, je));
    }
  }
};
}  // namespace rmq
}  // namespace nvector

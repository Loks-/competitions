#pragma once

#include "common/base.h"
#include "common/vector/rmq/position_value.h"
#include "common/vector/rmq/ppt.h"

#include <algorithm>
#include <cmath>
#include <stack>
#include <vector>

namespace nvector {
namespace rmq {
// Similar to PPT but cache solutions for small blocks.
// O(N) memory, O(N) preprocessing time, O(1) request time
template <class TTValue>
class PPTCartesianTree {
 public:
  using TValue = TTValue;
  using TPositionValue = PositionValue<TValue>;

 protected:
  size_t n, k, l;
  std::vector<std::vector<size_t>> vcpq;
  std::vector<size_t> vlmask, vlpos;
  std::vector<TValue> vv;
  std::vector<std::vector<std::vector<size_t>>> vs;
  PPT<TValue> rmq;

 protected:
  void InitCPQ() {
    vcpq.resize(k + 1);
    vcpq[0].resize(1);
    vcpq[0][0] = 1;
    for (size_t q = 1; q <= k; ++q) {
      vcpq[q].resize(q + 1);
      vcpq[q][0] = 1;
      for (size_t p = 1; p < q; ++p)
        vcpq[q][p] = vcpq[q][p - 1] + vcpq[q - 1][p];
      vcpq[q][q] = vcpq[q][q - 1];
    }
  }

  size_t CPQ(size_t p, size_t q) const { return vcpq[q][p]; }
  size_t Catalan(size_t p) const { return CPQ(p, p); }

  size_t Mask(size_t b) const {
    size_t m = 0, q = k;
    thread_local std::stack<TValue> s;
    for (size_t i = 0; i < k; ++i) {
      auto& v = vv[b + i];
      for (; !s.empty() && (v < s.top()); s.pop()) {
        m += CPQ(k - i - 1, q--);
      }
      s.push(v);
    }
    for (; !s.empty();) s.pop();
    return m;
  }

  size_t GetMask(size_t mask, size_t b, size_t e) const {
    return vs[mask][e - b - 1][b];
  }

  TPositionValue GetBlock(size_t j, size_t b, size_t e) const {
    size_t p = j * k + GetMask(vlmask[j], b, e);
    return {p, vv[p]};
  }

 public:
  PPTCartesianTree() {}
  PPTCartesianTree(const std::vector<TValue>& v) { Build(v); }

  void Build(const std::vector<TValue>& v) {
    n = v.size();
    k = std::max<size_t>(size_t(0.25 * log2(double(n))), 1);
    if (k == 1) {
      rmq.Build(v);
      return;
    }
    InitCPQ();
    l = (n + k - 1) / k;
    n = k * l;
    vv = v;
    for (; vv.size() < n;) vv.push_back(v.back());
    vlmask.resize(l);
    vlpos.resize(l);
    std::vector<TValue> vlmin(l);
    vs.clear();
    vs.resize(Catalan(k));
    for (size_t i = 0; i < l; ++i) {
      size_t ik = i * k, pos = ik;
      TValue value = vv[ik];
      for (size_t j = 1; j < k; ++j) {
        if (vv[ik + j] < value) {
          pos = ik + j;
          value = vv[pos];
        }
      }
      vlpos[i] = pos;
      vlmin[i] = value;
      size_t mask = Mask(ik);
      vlmask[i] = mask;
      if (vs[mask].size() == 0) {
        auto& vm = vs[mask];
        vm.resize(k);
        vm[0].resize(k);
        for (size_t j = 0; j < k; ++j) vm[0][j] = j;
        for (size_t ii = 1; ii < k; ++ii) {
          vm[ii].resize(k - ii);
          for (size_t j = 0; j < k - ii; ++j)
            vm[ii][j] = (vv[ik + vm[ii - 1][j + 1]] < vv[ik + j])
                            ? vm[ii - 1][j + 1]
                            : j;
        }
      }
    }
    rmq.Build(vlmin);
  }

  TPositionValue Minimum(size_t b, size_t e) const {
    if (k == 1) return rmq.Minimum(b, e);
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

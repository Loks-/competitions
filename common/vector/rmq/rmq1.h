#pragma once

#include "common/base.h"
#include "common/vector/rmq/position_value.h"
#include "common/vector/rmq/ppt.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace nvector {
namespace rmq {
// Similar to PPT but for vectors with difference exactly 1 between elements.
// O(N) memory, O(N) preprocessing time, O(1) request time
template <class TTValue>
class RMQ1 {
 public:
  using TValue = TTValue;
  using TPositionValue = PositionValue<TValue>;

 protected:
  size_t n, k, l;
  std::vector<size_t> vlmask, vlpos;
  std::vector<TValue> vlmin;
  std::vector<std::vector<std::vector<TPositionValue>>> vpv;
  PPT<TValue> rmq;

 protected:
  TPositionValue GetMask(size_t mask, size_t b, size_t e) const {
    return vpv[mask][e - b - 1][b];
  }

  TPositionValue GetBlock(size_t j, size_t b, size_t e) const {
    auto pv = GetMask(vlmask[j], b, e);
    return {j * k + pv.pos, vlmin[j] + pv.value};
  }

 public:
  RMQ1() {}
  RMQ1(const std::vector<TValue>& v) { Build(v); }

  void Build(const std::vector<TValue>& v) {
    n = v.size();
    k = std::max<size_t>(size_t(0.5 * log(double(n))), 1);
    if (k == 1) {
      rmq.Build(v);
      return;
    }
    l = (n + k - 1) / k;
    n = k * l;
    std::vector<TValue> vt(v);
    for (; vt.size() < n;) vt.push_back(vt.back() + 1);
    vlmask.resize(l);
    vlpos.resize(l);
    vlmin.resize(l);
    vpv.clear();
    vpv.resize(size_t(1) << (k - 1));
    for (size_t i = 0; i < l; ++i) {
      size_t mask = 0, pos = i * k;
      TValue value = vt[i * k];
      for (size_t j = 1; j < k; ++j) {
        mask <<= 1;
        if (vt[i * k + j] < vt[i * k + j - 1]) {
          mask += 1;
          if (vt[i * k + j] < value) {
            pos = i * k + j;
            value = vt[i * k + j];
          }
        }
      }
      vlmask[i] = mask;
      vlpos[i] = pos;
      vlmin[i] = value;
      if (vpv[mask].size() == 0) {
        auto& vm = vpv[mask];
        vm.resize(k);
        vm[0].resize(k);
        for (size_t j = 0; j < k; ++j) vm[0][j] = {j, vt[i * k + j] - value};
        for (size_t ii = 1; ii < k; ++ii) {
          vm[ii].resize(k - ii);
          for (size_t j = 0; j < k - ii; ++j)
            vm[ii][j] = Merge(vm[0][j], vm[ii - 1][j + 1]);
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

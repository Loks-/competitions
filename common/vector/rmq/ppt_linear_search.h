#pragma once

#include "common/base.h"
#include "common/vector/rmq/position_value.h"
#include "common/vector/rmq/ppt.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace nvector {
namespace rmq {
// Similar to PPT but use linear search on small blocks without preprocessing.
// Block size ~ log(N).
// O(N) memory, O(N) preprocessing time, O(log N) request time
template <class TTValue>
class PPTLinearSearch {
 public:
  using TValue = TTValue;
  using TPositionValue = PositionValue<TValue>;

 protected:
  size_t n, k, l;
  std::vector<size_t> vlpos;
  std::vector<TValue> vv;
  PPT<TValue> rmq;

 public:
  PPTLinearSearch() {}
  PPTLinearSearch(const std::vector<TValue>& v, double block_size_scale = 0.5) {
    Build(v, block_size_scale);
  }

  void Build(const std::vector<TValue>& v, double block_size_scale = 0.5) {
    n = v.size();
    k = std::max<size_t>(size_t(block_size_scale * log2(double(n))), 1);
    if (k == 1) {
      rmq.Build(v);
      return;
    }
    vv = v;
    l = n / k;
    vlpos.resize(l);
    std::vector<TValue> vlmin(l);
    for (size_t i = 0; i < l; ++i) {
      size_t pos = i * k;
      TValue value = v[i * k];
      for (size_t j = 1; j < k; ++j) {
        if (v[i * k + j] < value) {
          pos = i * k + j;
          value = v[i * k + j];
        }
      }
      vlpos[i] = pos;
      vlmin[i] = value;
    }
    rmq.Build(vlmin);
  }

  TPositionValue Minimum(size_t b, size_t e) const {
    if (k == 1) return rmq.Minimum(b, e);
    assert(b < e);
    size_t ib = b / k, ie = e / k;
    TPositionValue pv{b, vv[b]};
    if (ib + 1 < ie) {
      for (size_t i = b + 1; i < (ib + 1) * k; ++i) {
        if (vv[i] < pv.value) {
          pv.pos = i;
          pv.value = vv[i];
        }
      }
      auto rm = rmq.Minimum(ib + 1, ie);
      if (rm.value < pv.value) {
        pv.pos = vlpos[rm.pos];
        pv.value = rm.value;
      }
      for (size_t i = ie * k; i < e; ++i) {
        if (vv[i] < pv.value) {
          pv.pos = i;
          pv.value = vv[i];
        }
      }
    } else {
      for (size_t i = b + 1; i < e; ++i) {
        if (vv[i] < pv.value) {
          pv.pos = i;
          pv.value = vv[i];
        }
      }
    }
    return pv;
  }
};
}  // namespace rmq
}  // namespace nvector

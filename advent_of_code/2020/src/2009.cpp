#include "common/stl/base.h"
#include "common/vector/read_all.h"

int main_2009() {
  auto v = nvector::ReadAll<uint64_t>();
  unsigned l = v[0];
  uint64_t target = 0;
  for (unsigned i = l + 2; i < v.size(); ++i) {
    bool ok = false;
    for (unsigned j = i - l - 1; !ok && (j < i); ++j) {
      for (unsigned k = j + 1; k < i; ++k) {
        if (v[j] + v[k] == v[i]) ok = true;
      }
    }
    if (!ok) {
      target = v[i];
      break;
    }
  }
  cout << target << endl;
  v[0] = 0;
  for (unsigned i = 1; i < v.size(); ++i) v[i] += v[i - 1];
  for (unsigned i = 0; i < v.size(); ++i) {
    for (unsigned j = i + 2; j < v.size(); ++j) {
      if ((v[j] - v[i]) == target) {
        uint64_t minx = v[i + 1] - v[i], maxx = minx;
        for (unsigned k = i + 1; k < j; ++k) {
          auto x = v[k + 1] - v[k];
          minx = min(x, minx);
          maxx = max(x, maxx);
        }
        cout << maxx + minx << endl;
      }
    }
  }
  return 0;
}

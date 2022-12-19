#include "common/stl/base.h"
#include "common/stl/hash/pair.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2217() {
  vector<vector<unsigned>> vvb{
      {15}, {2, 7, 2}, {7, 4, 4}, {1, 1, 1, 1}, {3, 3}};
  unsigned H = 9;
  uint64_t M = 1000000000000ull - 1;
  vector<unsigned> v(H, 127);
  auto vs = nvector::ReadLines();
  auto s = vs[0];
  vector<uint64_t> vh;
  using TKey = pair<uint64_t, uint32_t>;
  unordered_map<TKey, pair<uint64_t, uint64_t>> m;
  for (unsigned i = 0, j = 0;; ++i) {
    auto b = vvb[i % vvb.size()];
    for (auto& x : b) x <<= 2;
    unsigned k = v.size() + 3;
    v.insert(v.end(), 7, 0);
    for (bool fok = true; fok; --k) {
      if (s[(j++) % s.size()] == '<') {
        bool ok = true;
        for (unsigned l = 0; l < b.size(); ++l) {
          if (b[l] & (1 | (v[k + l] << 1))) ok = false;
        }
        if (ok) {
          for (auto& x : b) x >>= 1;
        }
      } else {
        bool ok = true;
        for (unsigned l = 0; l < b.size(); ++l) {
          if (b[l] & (64 | (v[k + l] >> 1))) ok = false;
        }
        if (ok) {
          for (auto& x : b) x <<= 1;
        }
      }
      for (unsigned l = 0; l < b.size(); ++l) {
        if (b[l] & v[k - 1 + l]) fok = false;
      }
    }
    for (unsigned l = 0; l < b.size(); ++l) v[k + 1 + l] |= b[l];
    while (v.back() == 0) v.pop_back();
    vh.push_back(v.size());
    if (i == 2021) cout << v.size() - H << endl;
    uint64_t tmask = 0;
    for (unsigned h = 1; h <= H; ++h) tmask = (tmask << 7) + v[v.size() - h];
    auto key = make_pair(tmask, ((j % s.size()) << 4) + (i % vvb.size()));
    if ((m.find(key) == m.end()) || (i < 10000)) {
      m[key] = {i, v.size()};
    } else {
      auto mv = m[key];
      uint64_t di = i - mv.first, dh = v.size() - mv.second;
      cout << ((M - mv.first) / di) * dh +
                  vh[mv.first + ((M - mv.first) % di)] - H
           << endl;
      break;
    }
  }
  return 0;
}

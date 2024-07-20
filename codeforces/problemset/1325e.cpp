#include "common/factorization/factorization.h"
#include "common/heap/ukvm/dheap.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/unique.h"

#include <unordered_map>

using THeap = heap::ukvm::DHeap<4, unsigned>;

int main_1325e() {
  unsigned N;
  cin >> N;
  auto v = nvector::Read<unsigned>(N);
  vector<vector<unsigned>> vvp;
  vector<unsigned> vua;
  vector<unsigned> vp;
  for (auto u : v) {
    vector<unsigned> uvp;
    unsigned ua = 1;
    for (auto& p : Factorize(u)) {
      if (p.power & 1) {
        ua *= p.prime;
        uvp.push_back(p.prime);
        vp.push_back(p.prime);
      }
    }
    assert(uvp.size() <= 2);
    if (ua == 1) {
      std::cout << "1" << std::endl;
      return 0;
    } else {
      vua.push_back(ua);
      vvp.push_back(uvp);
    }
  }
  nvector::UniqueUnsorted(vua);
  if (vua.size() < N) {
    std::cout << "2" << std::endl;
    return 0;
  }
  nvector::UniqueUnsorted(vp);
  vector<unsigned> vpc(vp.size(), 0);
  vector<vector<pair<unsigned, unsigned>>> vvp1(vp.size());
  vector<unordered_map<unsigned, unsigned>> vm(vp.size());
  for (auto& uvp : vvp) {
    auto p0 = uvp[0];
    auto it0 = lower_bound(vp.begin(), vp.end(), p0);
    auto i0 = it0 - vp.begin();
    if (uvp.size() == 1) {
      vvp1[i0].push_back({1, i0});
    } else {
      auto p1 = uvp[1];
      auto it1 = lower_bound(vp.begin(), vp.end(), p1);
      auto i1 = it1 - vp.begin();
      vpc[i0]++;
      vpc[i1]++;
      vm[i0][i1] = 1;
      vm[i1][i0] = 1;
    }
  }
  unsigned answer = N + 1;

  auto Merge = [&](vector<pair<unsigned, unsigned>>& l,
                   const vector<pair<unsigned, unsigned>>& r, unsigned d) {
    if (r.empty()) return;
    if (l.empty()) {
      l = r;
      for (auto& itl : l) itl.first += d;
      return;
    }
    for (auto itl : l) {
      for (auto itr : r) {
        if (itl.second != itr.second) {
          answer = min(answer, itl.first + itr.first + d);
        }
      }
    }
    for (auto itr : r) {
      l.push_back({itr.first + d, itr.second});
    }
    sort(l.begin(), l.end());
    if ((l.size() >= 2) && (l[0].second == l[1].second)) {
      if (l.size() >= 3) {
        l[1] = l[2];
      } else {
        l.resize(1);
      }
    }
    if (l.size() >= 3) l.resize(2);
  };

  THeap h(vpc, false);
  for (; !h.Empty();) {
    auto u = h.ExtractKey();
    for (auto it1 = vm[u].begin(); it1 != vm[u].end(); ++it1) {
      for (auto it2 = vm[u].begin(); it2 != it1; ++it2) {
        auto u1 = it1->first, u2 = it2->first, d12 = it1->second + it2->second;
        auto& dm = vm[u1][u2];
        if (dm > 0) {
          answer = min(answer, dm + d12);
          dm = min(dm, d12);
        } else {
          dm = d12;
        }
        vm[u2][u1] = dm;
      }
    }
    for (auto it1 : vm[u]) {
      auto u1 = it1.first;
      Merge(vvp1[u1], vvp1[u], it1.second);
      vm[u1].erase(u);
      h.Set(u1, vm[u1].size());
    }
  }
  std::cout << ((answer <= N) ? int(answer) : -1) << std::endl;
  return 0;
}

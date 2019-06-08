#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"
#include "common/unsigned_set.h"

int main_zilionim() {
  uint64_t B = PowU<uint64_t>(10, 10), N = PowU<uint64_t>(10, 12);
  UnsignedSet us(100);
  vector<pair<uint64_t, unsigned>> vp;
  vp.push_back({0, 0});
  for (vp.push_back({B - 1, 0}); vp.back().first < N;) {
    uint64_t K = vp.back().first + 1, KE = K + B;
    unsigned i = 0, j = vp.size() - 1;
    for (; vp[i].first + B <= K; ++i) {
      uint64_t r = K - vp[i].first - B;
      for (; (j > 0) && (vp[j - 1].first >= r);) --j;
      KE = min(KE, vp[i].first + vp[j].first + B);
      us.Insert(vp[i].second ^ vp[j].second);
      if (vp[i].first + B == K) break;
      --r;
      for (; (j > 0) && (vp[j - 1].first >= r);) --j;
      KE = min(KE, vp[i + 1].first + vp[j].first + B);
      us.Insert(vp[i + 1].second ^ vp[j].second);
    }
    for (i = 0; us.HasKey(i);) ++i;
    us.Clear();
    // cerr << KE << "\t" << i << endl;
    if (vp.back().second == i)
      vp.back().first = KE;
    else
      vp.push_back({KE, i});
  }

  unsigned T, W;
  cin >> T >> W;
  for (unsigned iT = 1; iT <= T; ++iT) {
    vector<pair<uint64_t, uint64_t>> v;
    v.push_back({1, N + 1});
    int64_t M;
    for (cin >> M; M > 0; cin >> M) {
      bool found = false;
      for (unsigned i = 0; i < v.size(); ++i) {
        if ((v[i].first <= M) && (v[i].second >= M + B)) {
          found = true;
          v.push_back({M + B, v[i].second});
          v[i].second = M;
          break;
        }
      }
      assert(found);
      unsigned e = 0;
      for (auto p : v) {
        auto it =
            lower_bound(vp.begin(), vp.end(),
                        make_pair<uint64_t, unsigned>(p.second - p.first, 0));
        e ^= it->second;
      }
      M = 0;
      if (e) {
        for (auto p : v) {
          auto it =
              lower_bound(vp.begin(), vp.end(),
                          make_pair<uint64_t, unsigned>(p.second - p.first, 0));
          if (it->second ^ e < it->second) {
            unsigned ite = it->second ^ e;
            uint64_t K = p.second - p.first;
            unsigned i = 0, j = vp.size() - 1;
            found = false;
            for (; vp[i].first + B <= K; ++i) {
              uint64_t r = K - vp[i].first - B;
              for (; (j > 0) && (vp[j - 1].first >= r);) --j;
              if (vp[i].second ^ vp[j].second == ite) {
                found = true;
                M = p.first + vp[i].first;
                break;
              }
              if (vp[i].first + B == K) break;
              --r;
              for (; (j > 0) && (vp[j - 1].first >= r);) --j;
              if (vp[i + 1].second ^ vp[j].second == ite) {
                found = true;
                M = p.first + vp[i].first + 1;
                break;
              }
            }
            // assert(found);
            if (found) break;
          }
        }
      }
      assert((e == 0) || (M > 0));
      if (M == 0) {
        random_shuffle(v.begin(), v.end());
        for (auto p : v) {
          if (p.second - p.first >= B) {
            M = p.first;
            break;
          }
        }
      }
      cout << M << endl;
      found = false;
      for (unsigned i = 0; i < v.size(); ++i) {
        if ((v[i].first <= M) && (v[i].second >= M + B)) {
          found = true;
          v.push_back({M + B, v[i].second});
          v[i].second = M;
          break;
        }
      }
      assert(found);
    }
  }
  return 0;
}

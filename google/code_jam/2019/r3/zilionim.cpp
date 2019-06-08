#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"
#include "common/unsigned_set.h"

int main_zilionim() {
  uint64_t B = PowU<uint64_t>(10, 10), N = PowU<uint64_t>(10, 12);
  UnsignedSet us(100);
  vector<pair<uint64_t, unsigned>> vp;
  vp.push_back({0, 0});
  vp.push_back({B - 1, 0});
  for (; vp.back().first < N;) {
    uint64_t K = vp.back().first + 1, KE = K + B;
    unsigned i = 0, j = vp.size() - 1;
    for (; vp[i].first + B <= K; ++i) {
      uint64_t r = K - vp[i].first - B;
      for (; (j > 0) && (vp[j - 1].first >= r);) --j;
      KE = min(KE, vp[i].first + vp[j].first + B);
      us.Insert(vp[i].second ^ vp[j].second);
    }
    for (i = 0; us.HasKey(i);) ++i;
    us.Clear();
    if (vp.back().second == i) {
      vp.back().first = KE;
    } else {
      vp.push_back({K, i});
      vp.push_back({KE, i});
    }
  }
  auto Eval = [&](uint64_t l) {
    return lower_bound(vp.begin(), vp.end(), make_pair(l, 0u))->second;
  };

  unsigned T, W;
  cin >> T >> W;
  for (unsigned iT = 1; iT <= T; ++iT) {
    vector<pair<uint64_t, uint64_t>> v;
    v.push_back({1, N + 1});
    int64_t M;

    auto Move = [&]() {
      for (unsigned i = 0; i < v.size(); ++i) {
        if ((v[i].first <= M) && (v[i].second >= M + B)) {
          v.push_back({M + B, v[i].second});
          v[i].second = M;
          break;
        }
      }
    };

    for (cin >> M; M > 0; cin >> M) {
      Move();
      unsigned e = 0;
      for (auto p : v) e ^= Eval(p.second - p.first);
      M = 0;
      if (e) {
        for (auto p : v) {
          unsigned e1 = Eval(p.second - p.first);
          if ((e1 ^ e) < e1) {
            unsigned er = e1 ^ e;
            uint64_t l = p.second - p.first;
            unsigned i = 0, j = vp.size() - 1;
            for (; vp[i].first + B <= l; ++i) {
              if ((vp[i].second ^ Eval(l - vp[i].first - B)) == er) {
                M = p.first + vp[i].first;
                break;
              }
            }
          }
        }
      } else {
        for (auto p : v) {
          if (p.second - p.first >= B) {
            M = p.first;
            break;
          }
        }
      }
      cout << M << endl;
      Move();
    }
  }
  return 0;
}

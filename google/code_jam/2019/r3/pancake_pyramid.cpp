#include "common/modular/static/modular_io.h"
#include "common/stl/full.h"
#include "common/vector/read.h"

using TModular = Modular<>;

int main_pancake_pyramid() {
  unsigned T, S;
  cin >> T;
  for (unsigned iT = 1; iT <= T; ++iT) {
    cin >> S;
    auto vs = ReadVector<uint64_t>(S);
    TModular s = 0;

    std::function<void(unsigned, unsigned)> Solve = [&](unsigned b,
                                                        unsigned e) -> void {
      if (e - b < 3) return;
      unsigned mvi = b;
      uint64_t mv = vs[mvi];
      for (unsigned i = b + 1; i < e; ++i) {
        if (mv < vs[i]) {
          mv = vs[i];
          mvi = i;
        }
      }
      TModular sml = 0, smr = 0;
      uint64_t sl = 0, sr = 0;
      stack<pair<uint64_t, unsigned>> ssl, ssr;
      ssl.push({mv, mvi});
      for (unsigned i = mvi; i > b; --i) {
        unsigned i1 = i - 1;
        for (; ssl.top().first < vs[i1];) {
          uint64_t x = ssl.top().first;
          ssl.pop();
          sl +=
              (ssl.top().second - i1 - 1) * (min(vs[i1], ssl.top().first) - x);
        }
        ssl.push({vs[i1], i1});
        sml += sl;
      }
      ssr.push({mv, mvi});
      for (unsigned i = mvi + 1; i < e; ++i) {
        for (; ssr.top().first < vs[i];) {
          uint64_t x = ssr.top().first;
          ssr.pop();
          sr += (i - ssr.top().second - 1) * (min(vs[i], ssr.top().first) - x);
        }
        ssr.push({vs[i], i});
        smr += sr;
      }
      s +=
          (sml + (mvi - b + 1)) * (smr + (e - mvi)) - (mvi - b + 1) * (e - mvi);
      Solve(b, mvi);
      Solve(mvi + 1, e);
    };

    Solve(0, vs.size());
    cout << "Case #" << iT << ": " << s << endl;
  }
  return 0;
}

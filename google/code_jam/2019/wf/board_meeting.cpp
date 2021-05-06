#include "common/stl/base.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>

int main_board_meeting() {
  unsigned T, N, M, R;
  string s;
  cin >> T >> N >> M >> R;
  for (unsigned iT = 1; iT <= T; ++iT) {
    auto Get = [](int x, int y) {
      cout << x << " " << y << endl;
      cin >> x;
      return x;
    };

    vector<int> vt;
    vector<vector<int>> vp(2);
    vector<unordered_set<int>> vs(2);
    vector<unordered_map<int, int>> va(2);
    for (int a = 0; a < 2; ++a) {
      auto GetA = [&](int x) { return Get(x, (1 - 2 * a) * x); };

      std::function<void(int, int, int, int, int, int)> FindAll = [&](
          int lx, int lr, int dlr, int rx, int rr, int drr) -> void {
        if (dlr == drr) return;
        if (rx > lx + 2) {
          int mx1 = (lx + rx) / 2, mx2 = mx1 + 1, mr1 = GetA(mx1),
              mr2 = GetA(mx2);
          FindAll(lx, lr, dlr, mx1, mr1, mr2 - mr1);
          FindAll(mx2, mr2, mr2 - mr1, rx, rr, drr);
        } else if (rx == lx + 2) {
          int mx = lx + 1, mr = GetA(mx);
          FindAll(lx, lr, dlr, mx, mr, rr - mr);
          FindAll(rx, rr, rr - mr, rx, rr, drr);
        } else if (rx == lx + 1) {
          FindAll(lx, lr, dlr, lx, lr, rr - lr);
          FindAll(rx, rr, rr - lr, rx, lr, drr);
        } else {  // rx == lx
          vt.insert(vt.end(), drr - dlr, lx);
        }
      };

      int lx = -M - 1, lr = GetA(lx), rx = M + 1, rr = GetA(rx);
      vt.clear();
      FindAll(lx, lr, lr - GetA(lx - 1), rx, rr, GetA(rx + 1) - rr);

      for (size_t i = 0; i < vt.size(); i += 2) {
        int x = vt[i] + vt[i + 1];
        vp[a].push_back(x);
        vs[a].insert(x + 1);
      }
    }

    auto Calc = [&](int c, int d) {
      int r = 0;
      for (int a = 0; a < 2; ++a) {
        int e = c + (1 - 2 * a) * d;
        for (int x : vp[a]) r += abs(e - x);
      }
      return r / 2;
    };

    for (int a = 0; a < 2; ++a) {
      for (auto v : vs[a]) {
        int x = v + 5 * M, y = (2 * a - 1) * 5 * M;
        va[a][v] = Get(x, y) - Calc(x, y);
      }
    }

    cout << "READY" << endl;
    for (;;) {
      cin >> s;
      if (s == "ERROR") return -1;
      if (s == "DONE") break;
      int c = stoi(s), d;
      cin >> d;
      cout << Calc(c, d) + va[0][c + d] + va[1][c - d] << endl;
    }
  }
  return 0;
}

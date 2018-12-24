// https://www.hackerrank.com/challenges/swap-nodes-algo

#include "common/stl/base.h"

#include <functional>

int main_swap_nodes_algo() {
  unsigned N;
  cin >> N;
  vector<int> vl(N + 1, -1), vr(N + 1, -1), vd(N + 1, -1);
  vector<bool> vh(N + 1, true);
  for (unsigned i = 1; i <= N; ++i) {
    int l, r;
    cin >> l >> r;
    vl[i] = l;
    vr[i] = r;
    if (l != -1) vh[l] = false;
    if (r != -1) vh[r] = false;
  }
  unsigned h = 0;
  for (unsigned i = 1; i < N; ++i) {
    if (vh[i]) {
      h = i;
      break;
    }
  }

  std::function<void(int, int)> InitD = [&](int c, int cd) -> void {
    if (c != -1) {
      vd[c] = cd;
      InitD(vl[c], cd + 1);
      InitD(vr[c], cd + 1);
    }
  };

  std::function<void(int)> Print = [&](int c) -> void {
    if (c != -1) {
      Print(vl[c]);
      cout << c << " ";
      Print(vr[c]);
    }
  };

  InitD(h, 1);
  unsigned T;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    int d;
    cin >> d;
    for (unsigned i = 1; i <= N; ++i) {
      if ((vd[i] % d) == 0) swap(vl[i], vr[i]);
    }
    Print(h);
    cout << endl;
  }
  return 0;
}

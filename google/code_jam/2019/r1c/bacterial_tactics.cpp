#include "common/stl/base.h"
#include "common/vector/read.h"

#include <functional>
#include <string>

int main_bacterial_tactics() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned r, c;
    cin >> r >> c;
    auto vs = ReadVector<string>(r);
    vector<int> vcache(1u << 16, -1);

    std::function<int(unsigned, unsigned, unsigned, unsigned)> solve =
        [&](unsigned rb, unsigned re, unsigned cb, unsigned ce) -> int {
      if ((rb >= re) || (cb >= ce)) return 0;
      unsigned h = rb | (re << 4) | (cb << 8) | (ce << 12);
      int& ch = vcache[h];
      if (ch >= 0) return ch;
      vector<int> v;
      for (unsigned ri = rb; ri < re; ++ri) {
        bool valid = true;
        for (unsigned ci = cb; ci < ce; ++ci) {
          if (vs[ri][ci] == '#') {
            valid = false;
            break;
          }
        }
        if (valid) {
          v.push_back(solve(rb, ri, cb, ce) ^ solve(ri + 1, re, cb, ce));
        }
      }
      for (unsigned ci = cb; ci < ce; ++ci) {
        bool valid = true;
        for (unsigned ri = rb; ri < re; ++ri) {
          if (vs[ri][ci] == '#') {
            valid = false;
            break;
          }
        }
        if (valid) {
          v.push_back(solve(rb, re, cb, ci) ^ solve(rb, re, ci + 1, ce));
        }
      }
      sort(v.begin(), v.end());
      ch = 0;
      for (int i : v) {
        if (ch == i) ++ch;
      }
      return ch;
    };

    unsigned win_moves = 0;
    for (unsigned ri = 0; ri < r; ++ri) {
      bool valid = true;
      for (unsigned ci = 0; ci < c; ++ci) {
        if (vs[ri][ci] == '#') {
          valid = false;
          break;
        }
      }
      if (valid && !(solve(0, ri, 0, c) ^ solve(ri + 1, r, 0, c)))
        win_moves += c;
    }
    for (unsigned ci = 0; ci < c; ++ci) {
      bool valid = true;
      for (unsigned ri = 0; ri < r; ++ri) {
        if (vs[ri][ci] == '#') {
          valid = false;
          break;
        }
      }
      if (valid && !(solve(0, r, 0, ci) ^ solve(0, r, ci + 1, c)))
        win_moves += r;
    }

    cout << "Case #" << it << ": " << win_moves << std::endl;
  }
  return 0;
}

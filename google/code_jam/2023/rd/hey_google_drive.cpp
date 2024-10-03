#include "common/stl/base.h"
#include "common/vector/make.h"
#include "common/vector/read.h"

#include <cctype>
#include <queue>
#include <string>
#include <unordered_map>

int main_hey_google_drive() {
  unsigned T, R, C;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    cin >> R >> C;
    string s0(C, '#');
    auto vs = nvector::Read<string>(R);
    vs.insert(vs.begin(), s0);
    vs.push_back(s0);
    for (auto& s : vs) s = '#' + s + '#';
    R += 2;
    C += 2;
    auto RC = R * C;
    unordered_map<char, unsigned> m;
    for (unsigned i = 0; i < R; ++i) {
      for (unsigned j = 0; j < C; ++j) {
        auto& c = vs[i][j];
        if (isalpha(c)) {
          m[c] = i * C + j;
          c = '.';
        }
      }
    }
    vector<string> vr;
    for (auto cd = 'A'; cd <= 'Z'; ++cd) {
      auto itd = m.find(cd);
      if (itd == m.end()) continue;
      auto kd = itd->second;
      auto g = nvector::Make<unsigned>(RC, RC);
      for (unsigned i = 0; i < R; ++i) {
        for (unsigned j = 0; j < C; ++j) {
          if (vs[i][j] != '.') continue;
          auto k = i * C + j;
          if (vs[i - 1][j] != '#') g[k][k - C] = 1;
          if (vs[i + 1][j] != '#') g[k][k + C] = 1;
          if (vs[i][j - 1] != '#') g[k][k - 1] = 1;
          if (vs[i][j + 1] != '#') g[k][k + 1] = 1;
        }
      }
      vector<unsigned> vd(RC);
      queue<unsigned> q;
      for (bool blast = false; !blast;) {
        blast = true;
        fill(vd.begin(), vd.end(), RC);
        vd[kd] = 0;
        for (q.push(kd); !q.empty(); q.pop()) {
          auto k = q.front();
          for (unsigned x : {-C, -1u, 1u, C}) {
            auto k2 = k + x;
            if (g[k2][k] && (vd[k2] == RC)) {
              vd[k2] = vd[k] + 1;
              q.push(k2);
            }
          }
        }
        for (unsigned k = 0; k < R * C; ++k) {
          if ((vd[k] == RC) || (vd[k] == 0)) continue;
          if (g[k][k - C] && g[k][k + C]) {
            if ((vd[k - C] == RC) || (vd[k + C] == RC)) {
              g[k][k - C] = g[k][k + C] = 0;
              blast = false;
            }
          }
          if (g[k][k - 1] && g[k][k + 1]) {
            if ((vd[k - 1] == RC) || (vd[k + 1] == RC)) {
              g[k][k - 1] = g[k][k + 1] = 0;
              blast = false;
            }
          }
        }
      }
      for (char cs = 'a'; cs <= 'z'; ++cs) {
        auto its = m.find(cs);
        if (its == m.end()) continue;
        if (vd[its->second] != RC) vr.push_back(string(1, cs) + string(1, cd));
      }
    }
    sort(vr.begin(), vr.end());
    cout << "Case #" << it << ":";
    for (auto& s : vr) cout << " " << s;
    cout << (vr.empty() ? " NONE" : "") << endl;
  }
  return 0;
}

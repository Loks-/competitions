// https://www.hackerrank.com/challenges/palindromes

#include "common/linear_algebra/float/lup_decomposition.h"
#include "common/linear_algebra/matrix.h"
#include "common/stl/base.h"

#include <iomanip>
#include <string>
#include <unordered_map>
#include <utility>

int main_palindromes() {
  auto PreNormalize = [](const string& s) {
    vector<pair<unsigned, char>> v;
    for (char c = 'a'; c <= 'z'; ++c) v.push_back({0, c});
    for (char c : s) v[c - 'a'].first += 1;
    sort(v.begin(), v.end(), greater<pair<unsigned, char>>());
    vector<char> vm('z' + 1);
    for (unsigned i = 0; i < v.size(); ++i) vm[v[i].second] = 'a' + i;
    std::string snew;
    for (char c : s) snew += vm[c];
    return snew;
  };

  auto Normalize = [](const string& s) {
    unsigned l = unsigned(s.size());
    vector<string> v;
    for (unsigned i = 0; i < l / 2; ++i) {
      char c1 = s[i], c2 = s[l - i - 1];
      if (c1 <= c2)
        v.push_back(string() + c1 + c2);
      else
        v.push_back(string() + c2 + c1);
    }
    sort(v.begin(), v.end());
    string st = s;
    for (unsigned i = 0; i < l / 2; ++i) {
      st[i] = v[i][0];
      st[l - i - 1] = v[i][1];
    }
    return st;
  };

  cout << setprecision(4) << fixed;
  unsigned T;
  cin >> T;
  unordered_map<string, double> global_map;
  for (unsigned iT = 0; iT < T; ++iT) {
    string s;
    cin >> s;
    s = PreNormalize(s);
    string sn = Normalize(s);
    auto itp = global_map.find(sn);
    if (itp == global_map.end()) {
      unordered_map<string, unsigned> ms2u;
      unsigned l = unsigned(s.size()), n = 1;
      for (std::string st = s;;) {
        std::string st2 = Normalize(st);
        if (ms2u[st2] == 0) ms2u[st2] = n++;
        next_permutation(st.begin(), st.end());
        if (st == s) break;
      }
      TDMatrix m(n);
      m(0, 0) = 1.0;
      for (const auto& psu : ms2u) {
        string st = psu.first;
        unsigned k = psu.second;
        bool is_palindome = true;
        for (unsigned i = 0; i < l; ++i) {
          if (st[i] != st[l - i - 1]) {
            is_palindome = false;
            break;
          }
        }
        m(k, k) = 1.0;
        if (!is_palindome) {
          m(k, 0) = -1.0;
          double p = 2.0 / (l * (l - 1));
          for (unsigned i = 0; i < l; ++i) {
            for (unsigned j = i + 1; j < l; ++j) {
              swap(st[i], st[j]);
              string st2 = Normalize(st);
              unsigned k2 = ms2u[st2];
              m(k, k2) -= p;
              swap(st[i], st[j]);
            }
          }
        }
      }
      DLUPDecomposition<TDMatrix> lupd;
      lupd.Build(m);
      TDVector b(n), x(n);
      b(0) = 1.0;
      lupd.Solve(b, x);
      for (const auto& psu : ms2u) {
        string st = psu.first;
        unsigned k = psu.second;
        global_map[st] = x(k);
      }
    }
    cout << global_map[sn] << endl;
  }
  return 0;
}

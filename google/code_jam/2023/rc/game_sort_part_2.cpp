#include "common/stl/base.h"

#include <string>

int main_game_sort_part_2() {
  unsigned T, P;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    string s;
    vector<string> vs;
    cin >> P >> s;
    if (P == s.size()) {
      string ss = s;
      sort(ss.begin(), ss.end());
      if (s != ss) {
        for (unsigned i = 0; i < s.size(); ++i)
          vs.push_back(string(s.begin() + i, s.begin() + i + 1));
      }
    } else if (P == 2) {
      vector<pair<char, unsigned>> vl(s.size() - 1), vr(s.size() - 1);
      char ca = s[0], cz = s.back();
      vl[0] = {ca, 1u};
      for (unsigned i = 1; i + 1 < s.size(); ++i) {
        if (s[i] == ca) {
          vl[i] = {ca, vl[i - 1].second + 1u};
        } else if (s[i] < ca) {
          ca = s[i];
          vl[i] = {ca, 1u};
        } else {
          vl[i] = vl[i - 1];
        }
      }
      vr[s.size() - 2] = {cz, 1u};
      for (unsigned i = s.size() - 2; i-- > 0;) {
        if (s[i + 1] == cz) {
          vr[i] = {cz, vr[i + 1].second + 1};
        } else if (s[i + 1] > cz) {
          cz = s[i + 1];
          vr[i] = {cz, 1u};
        } else {
          vr[i] = vr[i + 1];
        }
      }
      for (unsigned i = 0; i < s.size() - 1; ++i) {
        auto pl = vl[i], pr = vr[i];
        if ((pl.first > pr.first) ||
            ((pl.first == pr.first) &&
             ((pl.second <= i) || (pl.second > pr.second)))) {
          vs.push_back(string(s.begin(), s.begin() + i + 1));
          vs.push_back(string(s.begin() + i + 1, s.end()));
          break;
        }
      }
    } else if (P == 3) {
      auto ca = s[0], cz = s.back();
      if (ca >= cz) {
        vs.push_back(string(s.begin(), s.begin() + 1));
        vs.push_back(string(s.begin() + 1, s.end() - 1));
        vs.push_back(string(s.end() - 1, s.end()));
      }
      if (vs.empty()) {
        for (unsigned i = 0; i < s.size(); ++i) {
          auto c = s[i];
          if ((c < ca) || ((c == ca) && (i > 1))) {
            vs.push_back(string(s.begin(), s.begin() + i));
            vs.push_back(string(s.begin() + i, s.begin() + i + 1));
            vs.push_back(string(s.begin() + i + 1, s.end()));
            break;
          }
        }
      }
      if (vs.empty()) {
        for (unsigned i = s.size(); i-- > 0;) {
          auto c = s[i];
          if (c > cz) {
            vs.push_back(string(s.begin(), s.begin() + i));
            vs.push_back(string(s.begin() + i, s.begin() + i + 1));
            vs.push_back(string(s.begin() + i + 1, s.end()));
            break;
          }
        }
      }
      if (vs.empty()) {
        vector<unsigned> vzc(s.size(), 0);
        for (unsigned i = 1; i < s.size(); ++i)
          vzc[i] = vzc[i - 1] + ((s[i] == cz) ? 1u : 0u);
        for (unsigned i = 1; i + 2 < s.size(); ++i) {
          if (s[i] != cz) continue;
          unsigned j = i + 1;
          for (; j + 1 < s.size(); ++j) {
            if (s[j] != cz) break;
          }
          if (vzc.back() - vzc[j - 1] < j - i) {
            vs.push_back(string(s.begin(), s.begin() + i));
            vs.push_back(string(s.begin() + i, s.begin() + j));
            vs.push_back(string(s.begin() + j, s.end()));
            break;
          } else {
            i = j;
          }
        }
      }
    } else {
      // P >= 4
      bool ok = false;
      string s1, s2, s3, s4;
      for (unsigned i = 0; i + 1 < s.size(); ++i) {
        if (s[i] > s[i + 1]) {
          ok = true;
          s1 = string(s.begin(), s.begin() + i);
          s2 = string(s.begin() + i, s.begin() + i + 1);
          s3 = string(s.begin() + i + 1, s.begin() + i + 2);
          s4 = string(s.begin() + i + 2, s.end());
          break;
        }
      }
      if (!ok) {
        for (unsigned i = 0; i + 2 < s.size(); ++i) {
          if (s[i] == s[i + 2]) {
            ok = true;
            s1 = string(s.begin(), s.begin() + i);
            s2 = string(s.begin() + i, s.begin() + i + 2);
            s3 = string(s.begin() + i + 2, s.begin() + i + 3);
            s4 = string(s.begin() + i + 3, s.end());
            break;
          }
        }
      }
      if (ok) {
        unsigned ml = P - 2 - (s4.empty() ? 0 : 1);
        if (s1.size() <= ml) {
          for (unsigned i = 0; i < s1.size(); ++i)
            vs.push_back(string(s1.begin() + i, s1.begin() + i + 1));
        } else {
          for (unsigned i = 0; i < ml - 1; ++i)
            vs.push_back(string(s1.begin() + i, s1.begin() + i + 1));
          vs.push_back(string(s1.begin() + ml - 1, s1.end()));
        }
        vs.push_back(s2);
        vs.push_back(s3);
        unsigned i = 0;
        for (; vs.size() < P - 1;) {
          vs.push_back(string(s4.begin() + i, s4.begin() + i + 1));
          ++i;
        }
        if (i < s4.size()) vs.push_back(string(s4.begin() + i, s4.end()));
      }
    }
    cout << "Case #" << it << ": " << (vs.empty() ? "IMPOSSIBLE" : "POSSIBLE")
         << endl;
    if (!vs.empty()) {
      for (auto& ss : vs) cout << ss << " ";
      cout << endl;
    }
  }
  return 0;
}

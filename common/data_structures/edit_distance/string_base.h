#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace ds {
namespace ed {
class StringBase {
 protected:
  std::vector<std::vector<unsigned>> vd;

 public:
  StringBase() {}
  StringBase(const std::string& s1, const std::string& s2) { Distance(s1, s2); }

  unsigned Distance(const std::string& s1, const std::string& s2) {
    vd.clear();
    vd.resize(s1.size() + 1, std::vector<unsigned>(s2.size() + 1, 0));
    for (unsigned i = 1; i <= s1.size(); ++i) vd[i][0] = i;
    for (unsigned j = 1; j <= s2.size(); ++j) vd[0][j] = j;
    for (unsigned i = 1; i <= s1.size(); ++i) {
      for (unsigned j = 1; j <= s2.size(); ++j) {
        vd[i][j] = (s1[i - 1] == s2[j - 1])
                       ? vd[i - 1][j - 1]
                       : 1 + std::min(std::min(vd[i - 1][j], vd[i][j - 1]),
                                      vd[i - 1][j - 1]);
      }
    }
    return vd[s1.size()][s2.size()];
  }

  std::vector<std::string> Sequence(const std::string& s1,
                                    const std::string& s2) {
    unsigned l = Distance(s1, s2);
    std::string s = s2;
    std::vector<std::string> vs(1, s);
    for (unsigned i = s1.size(), j = s2.size(); l; --l) {
      for (; (i > 0) && (j > 0) && (vd[i - 1][j - 1] == vd[i][j]) &&
             (s1[i - 1] == s2[j - 1]);
           --i)
        --j;
      if ((i > 0) && (1 + vd[i - 1][j] == vd[i][j])) {
        s.insert(s.begin() + j, s1[--i]);
      } else if ((j > 0) && (1 + vd[i][j - 1] == vd[i][j])) {
        s.erase(s.begin() + --j);
      } else {
        s[--j] = s1[--i];
      }
      vs.push_back(s);
    }
    return vs;
  }
};
}  // namespace ed
}  // namespace ds

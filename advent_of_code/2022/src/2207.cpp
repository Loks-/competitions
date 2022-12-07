#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <functional>
#include <numeric>
#include <unordered_map>

int main_2207() {
  class Dir {
   public:
    unsigned bsize = 0;
    vector<string> subdirs;
  };

  unordered_map<string, Dir> fs;
  string bdir = "/";

  string cdir = bdir;
  auto vs = nvector::ReadLines();
  for (unsigned i = 0; i < vs.size(); ++i) {
    auto& s = vs[i];
    auto ss = Split(s, " ");
    if (ss[1] == "cd") {
      if (ss[2] == bdir) {
        cdir = bdir;
      } else if (ss[2] == "..") {
        cdir.resize(cdir.find_last_of("/"));
      } else {
        cdir = cdir + "/" + ss[2];
      }
    } else if (ss[1] == "ls") {
      auto& d = fs[cdir];
      d.bsize = 0;
      d.subdirs.clear();
      for (; i + 1 < vs.size(); ++i) {
        auto& si = vs[i + 1];
        if (si[0] == '$') break;
        auto ss1 = Split(si, " ");
        if (ss1[0] == "dir") {
          d.subdirs.push_back(cdir + "/" + ss1[1]);
        } else {
          d.bsize += stoi(ss1[0]);
        }
      }
    }
  }

  vector<unsigned> vds;
  std::function<unsigned(const string&)> f = [&](const string& s) -> unsigned {
    auto d = fs[s];
    unsigned tsize = d.bsize;
    for (auto x : d.subdirs) tsize += f(x);
    vds.push_back(tsize);
    return tsize;
  };

  f(bdir);
  sort(vds.begin(), vds.end());
  auto it1 = upper_bound(vds.begin(), vds.end(), 100000);
  auto it2 = lower_bound(vds.begin(), vds.end(), vds.back() - 40000000);
  cout << accumulate(vds.begin(), it1, 0u) << endl << *it2 << endl;
  return 0;
}

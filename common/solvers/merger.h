#pragma once

#include "common/solvers/base.h"
#include "common/solvers/psolver.h"
#include <cassert>
#include <string>
#include <sys/stat.h>
#include <vector>

namespace solvers {
class Merger {
 protected:
  std::string dir_main, dir_sub;
  std::vector<PSolver> vsolvers;

 protected:
  static void MakeDir(const std::string& dir) { mkdir(dir.c_str(), S_IRWXU); }

 public:
  Merger(const std::string& main_dir, const std::string& sub_dir)
      : dir_main(main_dir), dir_sub(dir_main + "/" + sub_dir) {
    MakeDir(dir_main);
    MakeDir(dir_sub);
  }

  void Add(PSolver& s) {
    std::string dir_solutions =
        (s->UseSubDirectory() ? dir_sub : dir_main) + "/" + s->Name();
    MakeDir(dir_solutions);
    vsolvers.push_back(s->Clone());
  }

  void Solve(const std::string& problem) {
    // ...
  }
};
}  // namespace solvers

#pragma once

#include "common/solvers/base.h"
#include "common/solvers/psolver.h"
#include <cassert>
#include <filesystem>
#include <string>
#include <vector>

namespace solvers {
class Merger {
 protected:
  std::filesystem::path dir_main, dir_sub;
  std::vector<PSolver> vsolvers;

 public:
  Merger(const std::string& main_dir, const std::string& sub_dir)
      : dir_main(main_dir), dir_sub(dir_main / sub_dir) {}

  void Add(PSolver& s) {
    bool b = create_directories((s->UseSubDirectory() ? dir_sub : dir_main) /
                                s->Name());
    assert(b);
    vsolvers.push_back(s->Clone());
  }

  void Solve(const std::string& problem) {
    // ...
  }
};
}  // namespace solvers

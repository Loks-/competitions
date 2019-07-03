#pragma once

#include "common/solvers/base.h"
#include "common/solvers/psolver.h"
#include "common/timer.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>

namespace solvers {
class Merger {
 protected:
  std::string dir_main, dir_sub, dir_best;
  std::vector<PSolver> vsolvers;

 public:
  static void MakeDir(const std::string& dir) { mkdir(dir.c_str(), S_IRWXU); }

  static std::string Read(const std::string& dir,
                          const std::string& solution_file) {
    auto filename = dir + "/" + solution_file;
    std::ifstream f(filename);
    if (!f.is_open()) return "";
    std::string s;
    if (!std::getline(f, s)) return "";
    return s;
  }

  static void Write(const std::string& dir, const std::string& solution_file,
                    const std::string& solution) {
    auto filename = dir + "/" + solution_file;
    if (solution.empty()) return;
    std::ofstream f(filename);
    if (f.is_open()) f << solution;
  }

 public:
  Merger(const std::string& main_dir, const std::string& sub_dir)
      : dir_main(main_dir),
        dir_sub(dir_main + "/" + sub_dir),
        dir_best(dir_main + "/best") {
    MakeDir(dir_main);
    MakeDir(dir_sub);
    MakeDir(dir_best);
  }

  void Add(Base& s) {
    std::string dir_solutions =
        (s.UseSubDirectory() ? dir_sub : dir_main) + "/" + s.Name();
    MakeDir(dir_solutions);
    vsolvers.push_back(s.Clone());
  }

  template <class TEvaluator>
  void Solve(const std::string& problem, const std::string& solution_filename,
             const TEvaluator& evaluator) {
    Timer tproblem;
    auto best_results = evaluator(problem, Read(dir_best, solution_filename));
    std::string new_best_solution;
    for (PSolver psolver : vsolvers) {
      Timer tsolver;
      std::string solver_dir =
          (psolver->UseSubDirectory() ? dir_sub : dir_main) + "/" +
          psolver->Name();
      std::string solution;
      if (!psolver->SkipCacheRead()) {
        solution = Read(solver_dir, solution_filename);
      }
      if (solution.empty()) {
        solution = psolver->Solve(problem);
        if (!psolver->SkipCacheWrite())
          Write(solver_dir, solution_filename, solution);
      }
      auto solver_result = evaluator(problem, solution);
      if (!psolver->SkipBest() &&
          evaluator.Compare(solver_result, best_results)) {
        best_results = solver_result;
        new_best_solution = solution;
      }
      std::string name_adjusted = psolver->Name();
      if (name_adjusted.size() < 20) name_adjusted.resize(20, ' ');
      std::cout << solution_filename << "\t" << name_adjusted << "\t"
                << tsolver.GetMilliseconds() << "\t" << solver_result
                << std::endl;
    }
    if (!new_best_solution.empty()) {
      Write(dir_best, solution_filename, new_best_solution);
    }
    std::cout << solution_filename << "\ttotal               \t"
              << tproblem.GetMilliseconds() << "\t" << best_results
              << std::endl;
  }
};
}  // namespace solvers

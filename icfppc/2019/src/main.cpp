#include "base/eval/evaluator.h"
#include "command_line.h"
#include "solvers/lscm.h"
#include "solvers/single.h"
#include "solvers/worker/local.h"
#include "solvers/worker/simple.h"

#include "common/solvers/merger.h"
#include "common/thread_pool.h"

#include <algorithm>
#include <string>

void InitCommandLine() {
  cmd.AddArg(
      "threads",
      std::max(1, static_cast<int>(std::thread::hardware_concurrency()) - 1));
  cmd.AddArg("start", 1);
  cmd.AddArg("stop", 300);
}

int main(int argc, char* argv[]) {
  InitCommandLine();
  cmd.Parse(argc, argv);

  std::string path_to_root = "../icfppc/2019";
  base::eval::Evaluator evaluator;
  solvers::Merger merger(path_to_root + "/solutions", "all");
  solvers::LSCM solver_lscm;
  solvers::Single<solvers::worker::Simple> solver_single_simple;
  solvers::Single<solvers::worker::Local> solver_single_local;
  merger.Add(solver_lscm);
  merger.Add(solver_single_simple);
  merger.Add(solver_single_local);

  ThreadPool p(cmd.GetInt("threads"));
  for (int i = cmd.GetInt("start"); i <= cmd.GetInt("stop"); ++i) {
    auto t = std::make_shared<std::packaged_task<void()>>([&, i]() {
      std::string short_name = "prob-" + std::to_string(i + 1000).substr(1);
      std::string problem_dir = path_to_root + "/problems/main";
      std::string problem_filename = short_name + ".desc";
      std::string problem = merger.Read(problem_dir, problem_filename);
      std::string solution_filename = short_name + ".sol";
      merger.Solve(problem, solution_filename, evaluator);
    });
    p.enqueueTask(std::move(t));
  }
  return 0;
}

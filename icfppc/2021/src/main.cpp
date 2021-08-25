#include "settings.h"
#include "solvers/mctp_one.h"
#include "solvers/mctp_score.h"
#include "utils/evaluate_solution.h"

#include "common/files/command_line.h"
#include "common/solvers/ext/run_n.h"

void InitCommaneLine(files::CommandLine& cmd) {
  cmd.AddArg("mode", "eval");
  cmd.AddArg("solution", "best");
  cmd.AddArg("timelimit", 10);
  cmd.AddArg("nthreads", 4);
  cmd.AddArg("first_problem", 1);
  cmd.AddArg("last_problem", last_problem);
}

int main(int argc, char** argv) {
  files::CommandLine cmd;
  InitCommaneLine(cmd);
  cmd.Parse(argc, argv);

  auto mode = cmd.GetString("mode");
  if (mode == "eval") {
    EvaluateSolution(cmd.GetString("solution"));
  } else if (mode == "run") {
    src_solvers::MCTPOne s(cmd.GetInt("timelimit"));
    // src_solvers::MCTPScore s(cmd.GetInt("timelimit"));
    int nthreads = cmd.GetInt("nthreads");
    if (nthreads <= 0)
      solvers::ext::RunN<src_solvers::Base>(s, cmd.GetInt("first_problem"),
                                            cmd.GetInt("last_problem"));
    else
      solvers::ext::RunNMT<src_solvers::Base>(
          s, cmd.GetInt("first_problem"), cmd.GetInt("last_problem"), nthreads);
  } else {
    std::cerr << "Unknown mode " << mode << std::endl;
  }

  return 0;
}

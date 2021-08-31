#include "settings.h"
#include "solvers/full_search.h"
#include "solvers/full_search2.h"
#include "solvers/full_search3.h"
#include "solvers/mctp_one.h"
#include "solvers/mctp_score.h"
#include "solvers/mctp_vertices.h"
#include "utils/evaluate_solution.h"

#include "common/files/command_line.h"
#include "common/solvers/ext/run_n.h"

#include <memory>

void InitCommaneLine(files::CommandLine& cmd) {
  cmd.AddArg("mode", "eval");
  cmd.AddArg("solution", "best");
  cmd.AddArg("solver", "mctp_score");
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
    src_solvers::Base::PSolver s;
    auto solver_name = cmd.GetString("solver");
    auto timelimit = cmd.GetInt("timelimit");
    if (solver_name == "full_search") {
      s = std::make_shared<src_solvers::FullSearch>(timelimit);
    } else if (solver_name == "full_search2") {
      s = std::make_shared<src_solvers::FullSearch2>(timelimit);
    } else if (solver_name == "full_search3") {
      s = std::make_shared<src_solvers::FullSearch3>(timelimit);
    } else if (solver_name == "mctp_one") {
      s = std::make_shared<src_solvers::MCTPOne>(timelimit);
    } else if (solver_name == "mctp_score") {
      s = std::make_shared<src_solvers::MCTPScore>(timelimit);
    } else if (solver_name == "mctp_vertices") {
      s = std::make_shared<src_solvers::MCTPVertices>(timelimit);
    } else {
      std::cerr << "Unknown solver type: " << solver_name << std::endl;
      return -1;
    }
    int nthreads = cmd.GetInt("nthreads");
    if (nthreads <= 0)
      solvers::ext::RunN<src_solvers::Base>(*s, cmd.GetInt("first_problem"),
                                            cmd.GetInt("last_problem"));
    else
      solvers::ext::RunNMT<src_solvers::Base>(*s, cmd.GetInt("first_problem"),
                                              cmd.GetInt("last_problem"),
                                              nthreads);
  } else {
    std::cerr << "Unknown mode " << mode << std::endl;
  }

  return 0;
}

#include "common/proxy_run_base.h"
#include "common/proxy_run_main.h"

#include <iostream>
#include <string>

inline int ProxyRunAB(const std::string& solution,
                      const std::string& input_file_name) {
  auto it = solutions_map.find(solution);
  if (it != solutions_map.end()) return ProxyRun(solution, input_file_name);
  int r = 0;
  r += ProxyRun(solution + "a", input_file_name);
  r += ProxyRun(solution + "b", input_file_name);
  return r;
}

// Copied from proxy_run_main.h
inline int ProxyTestOneAB(const std::string& solution,
                          const std::string& input_file_name,
                          const std::string& test_file_name) {
  std::stringstream solution_output, expected_output;
  auto cout_backup = std::cout.rdbuf(solution_output.rdbuf());
  int solution_results = ProxyRunAB(solution, input_file_name);
  if (solution_results) return solution_results;
  std::cout.rdbuf(cout_backup);
  std::string sraw = solution_output.str(), sfixed;
  sfixed.reserve(sraw.size());
  for (unsigned i = 0; i < sraw.size(); ++i) {
    if ((sraw[i] == ' ') && ((sraw[i + 1] == ' ') || (sraw[i + 1] == '\n')))
      continue;
    sfixed.push_back(sraw[i]);
  }
  std::ifstream test_file(test_file_name);
  expected_output << test_file.rdbuf();
  if (sfixed != expected_output.str()) {
    std::cout << "Output for " << solution << " doesn't match expectation."
              << std::endl;
    return -3;
  }
  return 0;
}

inline int ProxyAutoAB(const std::string& solution,
                       const std::string& input_file_name,
                       const std::string& test_file_name) {
  return test_file_name.empty()
             ? ProxyRunAB(solution, input_file_name)
             : ProxyTestOneAB(solution, input_file_name, test_file_name);
}

int main(int nargs, char** pargs) {
  std::string solution_name;
  if (nargs >= 2) {
    solution_name = pargs[1];
  } else {
    std::cout << "Input advent of code solution to run:" << std::endl;
    std::cin >> solution_name;
  }
  return ProxyAutoAB(solution_name, (nargs > 2) ? pargs[2] : "",
                     (nargs > 3) ? pargs[3] : "");
}

#pragma once

#include "proxy_run_base.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

inline int proxy_run(const std::string& solution) {
  auto it = solutions_map.find(solution);
  if (it != solutions_map.end()) return it->second();
  std::cout << "Unknown solution name" << std::endl;
  return -2;
}

inline int proxy_run(const std::string& solution,
                     const std::string& input_file_name) {
  if (input_file_name.empty()) return proxy_run(solution);
  std::ifstream input_file(input_file_name);
  auto cin_backup = std::cin.rdbuf(input_file.rdbuf());
  int solution_results = proxy_run(solution);
  std::cin.rdbuf(cin_backup);
  return solution_results;
}

inline int proxy_run(const std::string& solution,
                     const std::string& input_file_name,
                     const std::string& output_file_name) {
  if (output_file_name.empty()) return proxy_run(solution, input_file_name);
  std::ofstream output_file(output_file_name);
  auto cout_backup = std::cout.rdbuf(output_file.rdbuf());
  int solution_results = proxy_run(solution, input_file_name);
  std::cout.rdbuf(cout_backup);
  return solution_results;
}

inline int proxy_test_one(const std::string& solution,
                          const std::string& input_file_name,
                          const std::string& test_file_name) {
  std::stringstream solution_output, expected_output;
  auto cout_backup = std::cout.rdbuf(solution_output.rdbuf());
  int solution_results = proxy_run(solution, input_file_name);
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

inline int proxy_test_all(const std::string& problem,
                          const std::string& input_file_name,
                          const std::string& test_file_name) {
  std::string problem_prefix = problem + "__";
  unsigned correct_solutions = 0;
  for (auto it = solutions_map.lower_bound(problem); it != solutions_map.end();
       ++it) {
    if ((it->first != problem) &&
        (it->first.substr(0, problem_prefix.size()) != problem_prefix))
      break;
    int solutions_results =
        proxy_test_one(it->first, input_file_name, test_file_name);
    if (solutions_results) return solutions_results;
    ++correct_solutions;
  }
  if (!correct_solutions) {
    std::cout << "No solution were found" << std::endl;
    return -4;
  }
  return 0;
}

int proxy_auto(const std::string& solution, const std::string& input_file_name,
               const std::string& test_file_name) {
  return test_file_name.empty()
             ? proxy_run(solution, input_file_name)
             : proxy_test_all(solution, input_file_name, test_file_name);
}

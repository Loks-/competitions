#pragma once

#include "common/proxy_run_base.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/**
 * @brief Runs the solution by name.
 *
 * @param solution The name of the solution to run.
 * @return The result of the solution execution.
 */
inline int ProxyRun(const std::string& solution) {
  const auto it = solutions_map.find(solution);
  if (it != solutions_map.end()) {
    return it->second();
  }
  std::cout << "Unknown solution name" << std::endl;
  return -2;
}

/**
 * @brief Runs the solution by name with input from a file.
 *
 * @param solution The name of the solution to run.
 * @param input_file_name The name of the input file.
 * @return The result of the solution execution.
 */
inline int ProxyRun(const std::string& solution,
                    const std::string& input_file_name) {
  if (input_file_name.empty()) {
    return ProxyRun(solution);
  }
  std::ifstream input_file(input_file_name);
  auto cin_backup = std::cin.rdbuf(input_file.rdbuf());
  const int solution_results = ProxyRun(solution);
  std::cin.rdbuf(cin_backup);
  return solution_results;
}

/**
 * @brief Runs the solution by name with input from a file and output to a file.
 *
 * @param solution The name of the solution to run.
 * @param input_file_name The name of the input file.
 * @param output_file_name The name of the output file.
 * @return The result of the solution execution.
 */
inline int ProxyRun(const std::string& solution,
                    const std::string& input_file_name,
                    const std::string& output_file_name) {
  if (output_file_name.empty()) {
    return ProxyRun(solution, input_file_name);
  }
  std::ofstream output_file(output_file_name);
  auto cout_backup = std::cout.rdbuf(output_file.rdbuf());
  const int solution_results = ProxyRun(solution, input_file_name);
  std::cout.rdbuf(cout_backup);
  return solution_results;
}

/**
 * @brief Tests a single solution against an expected output.
 *
 * @param solution The name of the solution to test.
 * @param input_file_name The name of the input file.
 * @param test_file_name The name of the file containing the expected output.
 * @return 0 if the solution output matches the expected output, otherwise an
 * error code.
 */
inline int ProxyTestOne(const std::string& solution,
                        const std::string& input_file_name,
                        const std::string& test_file_name) {
  std::stringstream solution_output, expected_output;
  auto cout_backup = std::cout.rdbuf(solution_output.rdbuf());
  const int solution_results = ProxyRun(solution, input_file_name);
  if (solution_results) {
    return solution_results;
  }
  std::cout.rdbuf(cout_backup);

  // Remove extra spaces and newlines from the solution output
  std::string sraw = solution_output.str(), sfixed;
  sfixed.reserve(sraw.size());
  for (unsigned i = 0; i < sraw.size(); ++i) {
    if ((sraw[i] == ' ') && ((sraw[i + 1] == ' ') || (sraw[i + 1] == '\n'))) {
      continue;
    }
    sfixed.push_back(sraw[i]);
  }

  // Read the expected output from the test file
  std::ifstream test_file(test_file_name);
  expected_output << test_file.rdbuf();
  if (sfixed != expected_output.str()) {
    std::cout << "Output for " << solution << " doesn't match expectation."
              << std::endl;
    return -3;
  }
  return 0;
}

/**
 * @brief Tests all solutions for a given problem against an expected output.
 *
 * @param problem The name of the problem to test.
 * @param input_file_name The name of the input file.
 * @param test_file_name The name of the file containing the expected output.
 * @return 0 if all solutions output match the expected output, otherwise an
 * error code.
 */
inline int ProxyTestAll(const std::string& problem,
                        const std::string& input_file_name,
                        const std::string& test_file_name) {
  const std::string problem_prefix = problem + "__";
  unsigned correct_solutions = 0;
  for (auto it = solutions_map.lower_bound(problem); it != solutions_map.end();
       ++it) {
    if ((it->first != problem) &&
        (it->first.substr(0, problem_prefix.size()) != problem_prefix)) {
      break;
    }
    const int solutions_results =
        ProxyTestOne(it->first, input_file_name, test_file_name);
    if (solutions_results) {
      return solutions_results;
    }
    ++correct_solutions;
  }
  if (!correct_solutions) {
    std::cout << "No solution were found" << std::endl;
    return -4;
  }
  return 0;
}

/**
 * @brief Automatically runs or tests a solution based on the presence of a test
 * file.
 *
 * @param solution The name of the solution to run or test.
 * @param input_file_name The name of the input file.
 * @param test_file_name The name of the file containing the expected output.
 * @return The result of the solution execution or testing.
 */
inline int ProxyAuto(const std::string& solution,
                     const std::string& input_file_name,
                     const std::string& test_file_name) {
  return test_file_name.empty()
             ? ProxyRun(solution, input_file_name)
             : ProxyTestAll(solution, input_file_name, test_file_name);
}

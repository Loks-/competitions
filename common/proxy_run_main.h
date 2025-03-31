#pragma once

#include "common/proxy_run_base.h"

#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/**
 * @brief Runs the solution by name.
 *
 * @param solution The name of the solution to run.
 * @return The result of the solution execution.
 *         Returns -1 if the solution throws an exception.
 *         Returns -2 if the solution is not found.
 *         Returns the solution's return value otherwise.
 */
inline int run_solution(const std::string& solution) {
  const auto it = solutions_map.find(solution);
  if (it != solutions_map.end()) {
    try {
      return it->second();
    } catch (const std::exception& e) {
      std::cerr << std::format("Solution execution failed: {}", e.what())
                << std::endl;
      return -1;
    }
  }
  std::cerr << std::format("Unknown solution name: {}", solution) << std::endl;
  return -2;
}

/**
 * @brief Runs the solution by name with input from a file.
 *
 * @param solution The name of the solution to run.
 * @param input_file_name The name of the input file.
 * @return The result of the solution execution.
 *         Returns -1 if the solution throws an exception.
 *         Returns -2 if the solution is not found.
 *         Returns -3 if the input file cannot be opened.
 *         Returns the solution's return value otherwise.
 */
inline int run_solution(const std::string& solution,
                        const std::string& input_file_name) {
  if (input_file_name.empty()) {
    return run_solution(solution);
  }
  std::ifstream input_file(input_file_name);
  if (!input_file.is_open()) {
    std::cerr << std::format("Failed to open input file: {}", input_file_name)
              << std::endl;
    return -3;
  }
  auto cin_backup = std::cin.rdbuf(input_file.rdbuf());
  const int solution_results = run_solution(solution);
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
 *         Returns -1 if the solution throws an exception.
 *         Returns -2 if the solution is not found.
 *         Returns -3 if the input file cannot be opened.
 *         Returns -4 if the output file cannot be opened.
 *         Returns the solution's return value otherwise.
 */
inline int run_solution(const std::string& solution,
                        const std::string& input_file_name,
                        const std::string& output_file_name) {
  if (output_file_name.empty()) {
    return run_solution(solution, input_file_name);
  }
  std::ofstream output_file(output_file_name);
  if (!output_file.is_open()) {
    std::cerr << std::format("Failed to open output file: {}", output_file_name)
              << std::endl;
    return -4;
  }
  auto cout_backup = std::cout.rdbuf(output_file.rdbuf());
  const int solution_results = run_solution(solution, input_file_name);
  std::cout.rdbuf(cout_backup);
  return solution_results;
}

/**
 * @brief Tests a single solution against an expected output.
 *
 * @param solution The name of the solution to test.
 * @param input_file_name The name of the input file.
 * @param test_file_name The name of the file containing the expected output.
 * @return The result of the test execution.
 *         Returns -1 if the solution throws an exception.
 *         Returns -2 if the solution is not found.
 *         Returns -3 if the input file cannot be opened.
 *         Returns -5 if the test file cannot be opened.
 *         Returns -6 if the solution output doesn't match the expected output.
 *         Returns 0 if the test passes successfully.
 */
inline int test_solution(const std::string& solution,
                         const std::string& input_file_name,
                         const std::string& test_file_name) {
  std::stringstream solution_output, expected_output;
  auto cout_backup = std::cout.rdbuf(solution_output.rdbuf());
  const int solution_results = run_solution(solution, input_file_name);
  if (solution_results) return solution_results;
  std::cout.rdbuf(cout_backup);

  // Remove extra spaces and newlines from the solution output
  const std::string sraw = solution_output.str();
  std::string sfixed;
  sfixed.reserve(sraw.size());
  for (unsigned i = 0; i < sraw.size(); ++i) {
    if ((sraw[i] == ' ') && ((sraw[i + 1] == ' ') || (sraw[i + 1] == '\n')))
      continue;
    sfixed.push_back(sraw[i]);
  }

  // Read the expected output from the test file
  std::ifstream test_file(test_file_name);
  if (!test_file.is_open()) {
    std::cerr << std::format("Failed to open test file: {}", test_file_name)
              << std::endl;
    return -5;
  }
  expected_output << test_file.rdbuf();
  if (sfixed != expected_output.str()) {
    std::cerr << std::format("Output for {} doesn't match expectation",
                             solution)
              << std::endl;
    return -6;
  }
  return 0;
}

/**
 * @brief Tests all solutions for a given problem against an expected output.
 *
 * @param problem The name of the problem to test.
 * @param input_file_name The name of the input file.
 * @param test_file_name The name of the file containing the expected output.
 * @return The result of the test execution.
 *         Returns -1 if any solution throws an exception.
 *         Returns -2 if no solutions are found.
 *         Returns -3 if the input file cannot be opened.
 *         Returns -5 if the test file cannot be opened.
 *         Returns -6 if any solution output doesn't match the expected output.
 *         Returns -7 if no solutions were found for the given problem.
 *         Returns 0 if all tests pass successfully.
 */
inline int test_all_solutions(const std::string& problem,
                              const std::string& input_file_name,
                              const std::string& test_file_name) {
  const std::string problem_prefix = problem + "__";
  unsigned correct_solutions = 0;
  for (auto it = solutions_map.lower_bound(problem); it != solutions_map.end();
       ++it) {
    if ((it->first != problem) &&
        (it->first.substr(0, problem_prefix.size()) != problem_prefix))
      break;
    const int solutions_results =
        test_solution(it->first, input_file_name, test_file_name);
    if (solutions_results) return solutions_results;
    ++correct_solutions;
  }
  if (!correct_solutions) {
    std::cerr << std::format("No solutions were found for problem: {}", problem)
              << std::endl;
    return -7;
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
 * @return The result of the execution or testing.
 *         Returns -1 if the solution throws an exception.
 *         Returns -2 if the solution is not found.
 *         Returns -3 if the input file cannot be opened.
 *         Returns -4 if the output file cannot be opened (in run mode).
 *         Returns -5 if the test file cannot be opened (in test mode).
 *         Returns -6 if the solution output doesn't match the expected output.
 *         Returns -7 if no solutions were found for the given problem.
 *         Returns 0 if the execution or testing is successful.
 */
inline int auto_run_or_test(const std::string& solution,
                            const std::string& input_file_name,
                            const std::string& test_file_name) {
  return test_file_name.empty()
             ? run_solution(solution, input_file_name)
             : test_all_solutions(solution, input_file_name, test_file_name);
}

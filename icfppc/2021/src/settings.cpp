#include "settings.h"

#include <string>

std::string ProblemFilename(const std::string& problem_id) {
  return path_to_root + "problems/" + problem_id + ".json";
}

std::string ProblemFilename(unsigned problem_id) {
  return ProblemFilename(std::to_string(problem_id));
}

std::string SolutionFilename(const std::string& problem_id,
                             const std::string& solver_name) {
  return path_to_root + "solutions/" + solver_name + "/" + problem_id + ".json";
}

std::string SolutionFilename(unsigned problem_id,
                             const std::string& solver_name) {
  return SolutionFilename(std::to_string(problem_id), solver_name);
}

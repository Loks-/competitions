#pragma once

#include <string>

namespace solvers {
class Problem {
 protected:
  std::string id;

 public:
  bool Load(const std::string& /* id */, const std::string& /* filename */);
};
}  // namespace solvers

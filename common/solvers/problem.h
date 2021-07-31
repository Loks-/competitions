#pragma once

#include <string>

namespace solvers {
class Problem {
 protected:
  std::string id;

 public:
  const std::string& Id() const { return id; }

  bool Load(const std::string& /* id */, const std::string& /* filename */);
};
}  // namespace solvers

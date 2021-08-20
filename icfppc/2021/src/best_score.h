#pragma once

#include <string>
#include <unordered_map>

class BestScore {
 protected:
  std::unordered_map<std::string, unsigned> score;

 public:
  BestScore();

  unsigned operator()(unsigned id) const;
  unsigned operator()(const std::string& id) const;
};

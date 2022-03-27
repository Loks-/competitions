#pragma once

#include "common/base.h"

#include <string>
#include <vector>

class TesterFixedUniverseSuccessor {
 protected:
  size_t usize;
  std::vector<size_t> vdata;

 public:
  TesterFixedUniverseSuccessor(size_t usize, size_t ssize);

 protected:
  template <class TFUS>
  size_t TestBase(const std::string& name) const;

 public:
  bool TestAll() const;
};

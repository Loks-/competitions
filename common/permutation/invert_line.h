#pragma once

#include <vector>

std::vector<unsigned> InvertLine(const std::vector<unsigned>& line) {
  std::vector<unsigned> v(line.size());
  for (unsigned i = 0; i < line.size(); ++i) v[line[i]] = i;
  return v;
}

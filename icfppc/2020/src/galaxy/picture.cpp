#include "picture.h"

#include <ostream>
#include <sstream>
#include <string>

void Picture::Clear() { v.clear(); }

void Picture::AddPixel(int64_t x, int64_t y) {
  v.push_back(std::make_pair(x, y));
}

std::string Picture::ToString() const {
  std::ostringstream ss;
  ss << "{ ";
  for (unsigned i = 0; i < v.size(); ++i) {
    ss << (i > 0 ? ", " : "") << "(" << v[i].first << " " << v[i].second << ")";
  }
  ss << " }";
  return ss.str();
}

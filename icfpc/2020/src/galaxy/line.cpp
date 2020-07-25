#include "galaxy/line.h"

#include <iostream>

namespace galaxy {
void Line::Print() {
  for (auto& e : v) {
    e.Print();
    std::cout << "  ";
  }
  std::cout << std::endl;
}
}  // namespace galaxy

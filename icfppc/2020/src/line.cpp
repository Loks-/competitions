#include "line.h"

#include <iostream>

void Line::Print() {
  for (auto& e : v) {
    e.Print();
    std::cout << "  ";
  }
  std::cout << std::endl;
}

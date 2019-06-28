#pragma once

#include <iostream>
#include <string>

class AssertException {};

inline void Assert(bool b) {
  if (!b) throw AssertException();
}

inline void Assert(bool b, const std::string& message) {
  if (!b) {
    std::cout << "Exception: " << message << std::endl;
    throw AssertException();
  }
}

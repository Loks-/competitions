#pragma once

#include "common/base.h"

#include <iostream>
#include <string>
#include <vector>

template <class TFunction, class TInput, class TOutput>
bool TestOne(const TFunction& f, const TInput& input, const TOutput& output) {
  auto r = f(input);
  if (r != output) {
    std::cout << "Test failed." << std::endl;
    return false;
  }
  return true;
}

template <class TFunction, class TInput, class TOutput>
bool TestAll(const TFunction& f, const std::vector<TInput>& input,
             const std::vector<TOutput>& output) {
  assert(input.size() == output.size());
  for (unsigned i = 0; i < input.size(); ++i) {
    if (!TestOne(f, input[i], output[i])) return false;
  }
  return true;
}

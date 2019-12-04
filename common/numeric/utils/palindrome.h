#pragma once

#include "common/base.h"
#include "common/numeric/utils/reverse.h"

bool Palindrome(uint64_t n, uint64_t base = 10) {
  return (n == Reverse(n, base));
}

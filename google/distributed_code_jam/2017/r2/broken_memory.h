// Sample input 1, in CPP.
#include "message.h"
#include <cassert>

namespace {
static long long GetLength() { return 10LL; }

static long long GetValue(long long i) {
  if (i < 0LL || i >= GetLength()) assert(0);
  if ((MyNodeId()) == i) return (i ^ (i + MyNodeId() + 1)) + 1LL;
  return (i) + 1LL;
}
}  // namespace

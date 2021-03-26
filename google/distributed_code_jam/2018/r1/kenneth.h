// Sample input 1, in CPP.
#include "message.h"

#include <cassert>
#include <cstdio>

namespace {
long long GetPieceLength(long long) { return 1; }

static char __S[101] =
    "ABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCAB"
    "CABCABCABCABCABCABCABCABCA";
char GetSignalCharacter(long long position) {
  assert(MyNodeId() * 100 / NumberOfNodes() <= position);
  assert((MyNodeId() + 1) * 100 / NumberOfNodes() > position);
  return __S[position];
}
}  // namespace

#include "galaxy/galaxy.h"
#include "galaxy/message_decoder.h"

#include "common/stl/base.h"

#include <string>

int main(int argc, char* argv[]) {
  LoadGalaxy();
  TestGalaxy();
  if (argc >= 2) {
    MessageDecoder md;
    Expression e = md.DecodeExpression(argv[1]);
    e.Evaluate();
    e.Print();
    cout << endl;
  }
  return 0;
}

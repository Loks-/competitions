#include "galaxy/galaxy.h"
#include "galaxy/message_decoder.h"

#include "common/stl/base.h"

#include <string>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cout << "Mode required. Supported modes are: [galaxy, galaxy_test]."
         << std::endl;
    return -1;
  }
  LoadGalaxy();
  string mode = argv[1];
  if (mode == "galaxy") {
    assert(argc > 2);
    MessageDecoder md;
    Expression e = md.DecodeExpression(argv[2]);
    e.Evaluate();
    e.Print();
    cout << endl;
  } else if (mode == "galaxy_test") {
    return TestGalaxy();
  } else {
    cout << "Unknown mode." << endl;
    return -1;
  }
  return 0;
}

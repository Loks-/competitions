#include "app/send.h"
#include "galaxy/evaluation.h"
#include "galaxy/galaxy.h"
#include "galaxy/message_decoder.h"
#include "tournament/referee.h"

#include "common/stl/base.h"

#include <string>

// Protocol commands examples:
//   Initial sequence:
//     1. "ap ap ap interact galaxy ( ) ap ap vec 0 0"
//     2. "ap ap ap interact galaxy ( 0 , ( 0 ) , 0 , ( ) ) ap ap vec 0 0"
//     3. "ap ap ap interact galaxy ( 0 , ( 1 ) , 0 , ( ) ) ap ap vec 0 0"
//     4. "ap ap ap interact galaxy ( 0 , ( 2 ) , 0 , ( ) ) ap ap vec 0 0"
//     5. "ap ap ap interact galaxy ( 0 , ( 3 ) , 0 , ( ) ) ap ap vec 0 0"
//     6. "ap ap ap interact galaxy ( 1 , ( 1 ) , 0 , ( ) ) ap ap vec 0 0"
//     7. "ap ap ap interact galaxy ( 1 , ( 2 ) , 0 , ( ) ) ap ap vec 0 0"
//     8. "ap ap ap interact galaxy ( 1 , ( 3 ) , 0 , ( ) ) ap ap vec 0 0"
//     9. "ap ap ap interact galaxy ( 1 , ( 4 ) , 0 , ( ) ) ap ap vec 8 4"
//    10. "ap ap ap interact galaxy ( 1 , ( 5 ) , 0 , ( ) ) ap ap vec 2 -8"
//    11. "ap ap ap interact galaxy ( 1 , ( 6 ) , 0 , ( ) ) ap ap vec 3 6"
//    12. "ap ap ap interact galaxy ( 1 , ( 7 ) , 0 , ( ) ) ap ap vec 0 -14"
//    13. "ap ap ap interact galaxy ( 1 , ( 8 ) , 0 , ( ) ) ap ap vec -4 10"
//    14. "ap ap ap interact galaxy ( 1 , ( 9 ) , 0 , ( ) ) ap ap vec 9 -3"
//    15. "ap ap ap interact galaxy ( 1 , ( 10 ) , 0 , ( ) ) ap ap vec -4 10"
//    16. "ap ap ap interact galaxy ( 1 , ( 11 ) , 0 , ( ) ) ap ap vec 1 4"
//   Galaxy: (most pixels are fine as vector)
//     "ap ap ap interact galaxy ( 2 , ( 1 , -1 ) , 0 , ( ) ) ap ap vec 1000
//     1000"
//   Tutorial:
//     "ap ap ap interact galaxy ( 5 , ( 2 , 1 , nil , nil , nil , nil , nil ,
//     44309 ) , 9 , ( 103652820 , 192496425430 ) ) ap ap vec 1000 1000"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cout << "Mode required. Supported modes are: [galaxy, galaxy_no_draw, "
            "galaxy_test play send]."
         << std::endl;
    return -1;
  }
  string mode = argv[1];
  if ((mode == "galaxy") || (mode == "galaxy_no_draw")) {
    assert(argc > 2);
    galaxy::LoadGalaxy();
    galaxy::EvaluationSkipDraw(mode == "galaxy_no_draw");
    galaxy::MessageDecoder md;
    auto e = md.DecodeExpression(argv[2]);
    e.Evaluate();
    e.Print();
    cout << endl;
  } else if (mode == "galaxy_test") {
    galaxy::LoadGalaxy();
    return galaxy::TestGalaxy();
  } else if (mode == "play") {
    tournament::Referee r;
    r.Play();
  } else if (mode == "send") {
    assert(argc > 2);
    app::Send(argv[2]);
  } else {
    cout << "Unknown mode." << endl;
    return -1;
  }
  return 0;
}

#include "message.h"
#include "message_as_text.h"
#include "message_decoder.h"

#include "common/base.h"

#include <iostream>
#include <string>

void TestAll(const std::string& filename) {
  MessageAsText mt(filename);
  MessageDecoder md;
  Message m = md.Decode(mt);
  for (auto& l : m.v) {
    // l.Print();
    // std::cout << std::endl;
    assert(l.v.size() == 3);
    assert(l.v[1].v.size() == 1);
    assert(l.v[1].v[0].type == GlyphType::EQUALITY);
    l.v[0].Evaluate();
    l.v[2].Evaluate();
    assert(l.v[0] == l.v[2]);
  }
}

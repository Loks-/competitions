#include "galaxy/message.h"

#include "galaxy/dictionary.h"

#include "common/base.h"

#include <algorithm>

namespace galaxy {
void Message::Process() {
  // Manual functions
  //   aliases[1117] = Expression(Glyph(FunctionType::POWER_OF_TWO));
  //   aliases[1118] = Expression(Glyph(FunctionType::LOG2));
  //   aliases[1128] = Expression(Glyph(FunctionType::LENGTH));
  //   aliases[1131] = Expression(Glyph(FunctionType::CONCAT));

  for (unsigned i = 0; i < v.size(); ++i) {
    assert(v[i].v.size() == 1);
    auto& vvv = v[i].v[0].v;
    assert(vvv.size() >= 3);
    assert(vvv[1].type == GlyphType::EQUALITY);
    if (vvv[0].ftype == FunctionType::GALAXY)
      vvv[0] = Glyph(GlyphType::ALIAS, 0);
    assert(vvv[0].type == GlyphType::ALIAS);
    Expression e;
    for (unsigned j = 2; j < vvv.size(); ++j) e.Add(vvv[j]);
    AddToDictionary(vvv[0].value, e.MakeRoot());
  }
}

void Message::Print() {
  for (auto& l : v) l.Print();
}
}  // namespace galaxy

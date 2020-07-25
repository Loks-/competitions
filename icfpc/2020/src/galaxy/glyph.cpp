#include "galaxy/glyph.h"

#include "galaxy/glyph_type.h"

#include <iostream>

namespace galaxy {
Glyph::Glyph()
    : type(GlyphType::UNKNOWN), ftype(FunctionType::NONE), value(0), lef(0) {}

Glyph::Glyph(GlyphType _type)
    : type(_type), ftype(FunctionType::NONE), value(0), lef(0) {}

Glyph::Glyph(FunctionType _ftype)
    : type(GlyphType::FUNCTION), ftype(_ftype), value(0), lef(0) {}

Glyph::Glyph(GlyphType _type, int64_t _value)
    : type(_type), ftype(FunctionType::NONE), value(_value), lef(0) {}

Glyph::Glyph(const LEF& _lef)
    : type(GlyphType::LINEAR_ENCODED_FORM),
      ftype(FunctionType::NONE),
      value(0),
      lef(_lef) {}

Glyph::Glyph(const Picture& _pic)
    : type(GlyphType::PICTURE),
      ftype(FunctionType::NONE),
      value(0),
      lef(0),
      pic(_pic) {}

bool Glyph::operator==(const Glyph& g) const {
  return (type == g.type) && (ftype == g.ftype) && (value == g.value) &&
         (lef == g.lef);
}

void Glyph::Print() const {
  switch (type) {
    case GlyphType::NUMBER:
      std::cout << value;
      break;
    case GlyphType::ONE:
      std::cout << "I";
      break;
    case GlyphType::DOT:
      std::cout << ".";
      break;
    case GlyphType::EQUALITY:
      std::cout << "===";
      break;
    case GlyphType::AP:
      std::cout << "^";
      break;
    case GlyphType::FUNCTION:
      std::cout << "F" << unsigned(ftype);
      break;
    case GlyphType::VARIABLE:
      std::cout << "V" << value;
      break;
    case GlyphType::LINEAR_ENCODED_FORM:
      std::cout << "L" << LEFDecodeNumber(lef);
      break;
    case GlyphType::ALIAS:
      std::cout << "A" << value;
      break;
    default:
      std::cout << "UNKNOWN";
      break;
  }
}
}  // namespace galaxy
